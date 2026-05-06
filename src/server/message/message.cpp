#include "message.hpp"
#include "../../common/protocol.hpp"
#include "../../project/Chat/userMessage.hpp"
#include "../../project/Chat/systemNotification.hpp"
#include "../datamanager/miniz.h"
#include "../reponse/reponse.hpp"
#include "../worker.hpp"
#include <filesystem>
#include <memory>
#include <SFML/Graphics/Texture.hpp>
#include "../../common/sfml_overload.hpp"

namespace fs = std::filesystem;
ConnectUserMessage::ConnectUserMessage(sf::Packet &dataPacket,
                                       std::shared_ptr<Client> client) {
  dataPacket >> pseudo_ >> password_;
  client_ = client;
}

LoginMessage::LoginMessage(sf::Packet &dataPacket,
                           std::shared_ptr<Client> client)
    : ConnectUserMessage(dataPacket, std::move(client)) {}

void LoginMessage::process(Worker &worker) {
  uint id = worker.verifyLogin(this->pseudo_, this->password_);
  if (id != -1) {
    client_->pseudo = this->pseudo_;
  }
  std::unique_ptr<Reponse> rps;
  rps = std::make_unique<ReponseAuth>(std::move(client_), id);
  worker.pushNetwork(std::move(rps));
}

RegisterMessage::RegisterMessage(sf::Packet &dataPacket,
                                 std::shared_ptr<Client> client)
    : ConnectUserMessage(dataPacket, std::move(client)) {}

void RegisterMessage::process(Worker &worker) {
  uint id = worker.addUser(this->pseudo_, this->password_);
  if (id != -1) {
    client_->pseudo = this->pseudo_;
  }
  std::unique_ptr<Reponse> rps;
  rps = std::make_unique<ReponseAuth>(std::move(client_), id);
  worker.pushNetwork(std::move(rps));
}

CreateProjectMessage::CreateProjectMessage(sf::Packet &data_packet,
                                           std::shared_ptr<Client> client) {
  data_packet >> nomProjet_ >> size_.x >> size_.y >> scale_;
  userId_ = client->id;
  client_ = std::move(client);
}

void CreateProjectMessage::process(Worker &worker) {
  if (userId_ > 0) {
    uint idProj = worker.addProjectSQL(nomProjet_, userId_);
    client_->projectId = idProj;

    auto liveProj = LiveProject(idProj, nomProjet_, size_.x, size_.y, scale_);
    liveProj.addConnection(userId_, 2);
    worker.mapProjet_.emplace(idProj, std::move(liveProj));

    std::unique_ptr<Reponse> rps;
    rps = std::make_unique<ReponseCreateProject>(userId_, idProj);
    worker.pushNetwork(std::move(rps));

    createSystemNotification(worker, idProj, client_->pseudo, userId_, typeNotification::CONNEXION);
  }
}
ExportNativeMessage::ExportNativeMessage(sf::Packet &data_packet,
                                         std::shared_ptr<Client> client) {
  data_packet >> projectId_;
  userId_ = client->id;
}

void ExportNativeMessage::process(Worker &worker) {
  auto itProject = worker.mapProjet_.find(projectId_);

  if (itProject == worker.mapProjet_.end()) {
    return;
  }

  if (userId_ > 0) {
    // Server saves project
    std::unique_ptr<ExportDemand> savetsk;
    savetsk = std::make_unique<ExportDemand>(worker.mapProjet_.at(projectId_),
                                             projectId_, userId_);
    worker.pushSave(std::move(savetsk));
  }
}

RenameProjectMessage::RenameProjectMessage(sf::Packet &dataPacket,
                                           std::shared_ptr<Client> &client) {
  dataPacket >> projectId_ >> newName_;
  userID_ = client->id;
}

void RenameProjectMessage::process(Worker &worker) {
  bool success = worker.renameProject(projectId_, newName_);

  std::unique_ptr<Reponse> rps;
  rps = std::make_unique<ReponseRenameProject>(userID_, projectId_, newName_,
                                               success);
  worker.pushNetwork(std::move(rps));
}

ImportProjectMessage::ImportProjectMessage(sf::Packet &dataPacket,
                                           std::shared_ptr<Client> &client) {
  userId_ = client->id;
  uint32_t dataSize;
  dataPacket >> dataSize >> projName_;
  // Same as in the handler for exportation
  const void *rawBuf = static_cast<const char *>(dataPacket.getData()) +
                       dataPacket.getReadPosition();
  file_ = QByteArray(static_cast<const char *>(rawBuf), dataSize);
}
/* Read raw data from zip in RAM without creating temporary files
 * In order to extract it in servers'RAM
 */
// Got help from AI for this use of miniz.c because lack of documentation
bool exportNativeFromMemory(const QByteArray &zipData,
                            const std::string &destPath) {
  // Creating empty zipFile (destination file)
  mz_zip_archive zip_archive;
  memset(&zip_archive, 0, sizeof(zip_archive));

  // Miniz reads raw data from RAM
  if (!mz_zip_reader_init_mem(&zip_archive, zipData.constData(), zipData.size(),
                              0)) {
    std::cerr << "Erreur : Impossible de lire l'archive ZIP depuis la mémoire."
              << std::endl;
    return false;
  }

  // Ensures Directories in destination Folder exists
  fs::create_directories(destPath);

  int nbFiles = mz_zip_reader_get_num_files(&zip_archive);

  // Iterating over every element in zip
  for (int i = 0; i < nbFiles; i++) {
    mz_zip_archive_file_stat infoFichier;
    // Fetch information of the file i (name, size, ...)
    if (!mz_zip_reader_file_stat(&zip_archive, i, &infoFichier))
      continue;

    // aboslute path (server's side)
    fs::path finalPath = fs::path(destPath) / infoFichier.m_filename;

    if (mz_zip_reader_is_file_a_directory(&zip_archive, i)) {
      fs::create_directories(finalPath);
      continue;
    }

    fs::create_directories(finalPath.parent_path());

    // Extracting file to server's file
    if (!mz_zip_reader_extract_to_file(&zip_archive, i,
                                       finalPath.string().c_str(), 0)) {
      std::cerr << "Erreur lors de l'extraction de : " << infoFichier.m_filename
                << std::endl;
    }
  }
  mz_zip_reader_end(&zip_archive);
  return true;
}
/*
 * Importing file from client to server
 * */
void ImportProjectMessage::process(Worker &worker) {
  uint newId = worker.addProjectSQL(projName_, userId_);

  QByteArray zipBytes = qUncompress(file_);

  std::string dossierDestination = "projectsFolder/project_" + to_string(newId);

  bool success = exportNativeFromMemory(zipBytes, dossierDestination);

  if (success) {
    // Updating imported project's name
    worker.getProjMngr().updateJsonDup(newId,
                                       QString::fromStdString(projName_));

    LiveProject liveProj = LiveProject(newId);
    // Adding user as owner
    liveProj.addConnection(userId_, 2);
    worker.mapProjet_.emplace(newId, std::move(liveProj));
    // Updating user's project list
    std::vector<ProjectEntry> projects = worker.getUserProjects(userId_);
    std::unique_ptr<Reponse> rps =
        std::make_unique<ReponseUsersProjects>(userId_, projects);
    std::cout <<"construction reponse" <<std::endl;
    worker.pushNetwork(std::move(rps));
  } else {
    std::cerr << "L'extraction du projet importé a échoué." << std::endl;
  }
}
DuplicateProjectMessage::DuplicateProjectMessage(
    sf::Packet &dataPacket, std::shared_ptr<Client> &client) {
  dataPacket >> projectId_ >> newName;
  userId_ = client->id;
}

void DuplicateProjectMessage::process(Worker &worker) {
  uint newId = worker.duplicateProject(projectId_, newName, userId_);
  std::unique_ptr<Reponse> rps;
  if (newId != -1) {
    rps = std::make_unique<ReponseDuplicateProject>(userId_, newId, newName);
    worker.pushNetwork(std::move(rps));
  }
}

GetProjectsListMessage::GetProjectsListMessage(
    sf::Packet &data_packet, std::shared_ptr<Client> &client) {
  userId_ = client->id;
}

void GetProjectsListMessage::process(Worker &worker) {
  std::vector<ProjectEntry> projects;
  if (userId_ != 0) {
    projects = worker.getUserProjects(userId_);
  }

  std::unique_ptr<Reponse> rps;
  rps = std::make_unique<ReponseUsersProjects>(userId_, projects);
  worker.pushNetwork(std::move(rps));
}

DeleteProjectMessage::DeleteProjectMessage(sf::Packet &data_packet,
                                           std::shared_ptr<Client> &client) {
  userId_ = client->id;
  data_packet >> projectId_;
}

void DeleteProjectMessage::process(Worker &worker) {
  if (worker.getRole(userId_, projectId_) == 2) {
    // if owner -> delete project for everybody
    worker.deleteProject(projectId_);
  } else {
    worker.removeLink(userId_, projectId_);
  }
}

GetProjectDataMessage::GetProjectDataMessage(sf::Packet &data_packet,
                                             std::shared_ptr<Client> &client) {
  userId_ = client->id;
  data_packet >> projectId_;
  client->projectId = projectId_;
  pseudo_ = client->pseudo;
}

LeaveProjectMessage::LeaveProjectMessage(sf::Packet &data_packet, uint userId) {
  userId_ = userId;
  data_packet >> projectId_;
}
void LeaveProjectMessage::process(Worker &worker) {
  worker.removeLink(userId_, projectId_);
}

KickUserMessage::KickUserMessage(sf::Packet &data_packet, uint userId) {
  userId_ = userId;
  data_packet >> targetId_ >> projectId_;
}
void KickUserMessage::process(Worker &worker) {
  bool success = worker.removeLink(targetId_, projectId_);
  // Getting all connected users
  std::vector<uint> usersId;
  auto it = worker.mapProjet_.find(projectId_);
  if (it != worker.mapProjet_.end()) {
    usersId = it->second.getConnected();
  }
  if (std::find(usersId.begin(), usersId.end(), targetId_) == usersId.end()) {
    std::cout << "l'utilisateur n'était pas connecté au projet" << std::endl;
    usersId.push_back(targetId_);
  }
  // Building the group response
  std::unique_ptr<Reponse> rps;
  rps = std::make_unique<ReponseKickUserProject>(usersId, targetId_, projectId_,
                                                 success);

  std::cout << "Reussi a kick( message): " << success << std::endl;
  worker.pushNetwork(std::move(rps));
}

GetMemberMessage::GetMemberMessage(sf::Packet &data_packet, uint userId) {
  userId_ = userId;
  data_packet >> projectId_;
}

void GetMemberMessage::process(Worker &worker) {
  std::vector<MemberEntry> memberList = worker.getProjectMembers(projectId_);

  std::unique_ptr<Reponse> rps;
  rps = std::make_unique<ReponseGetMember>(userId_, memberList);
  worker.pushNetwork(std::move(rps));
}

void GetProjectDataMessage::process(Worker &worker) {
  int8_t role = worker.getRole(userId_, projectId_);
  if (role != -1) {
    if (worker.mapProjet_.find(projectId_) == worker.mapProjet_.end()) {
      LiveProject liveProj = LiveProject(projectId_);
      worker.mapProjet_.emplace(projectId_, std::move(liveProj));
    }
    LiveProject &liveProj = worker.mapProjet_.at(projectId_);
    liveProj.addConnection(userId_, role); // WARNING: LE 1 EST FORCE CODER
    
    std::unique_ptr<Reponse> rps;
    rps = std::make_unique<ReponseProjectData>(
        userId_, liveProj.getJson(), std::move(liveProj.getLayerOrder()),
        liveProj.getImageMap(), liveProj.getSpritesMap(), liveProj.getChatJson());
    worker.pushNetwork(std::move(rps));

    createSystemNotification(worker, projectId_, pseudo_, userId_, typeNotification::CONNEXION);
  }
}

std::vector<uint> ModifProjetMessage::getUserLists(Worker &worker) {
  auto itProject = worker.mapProjet_.find(projectId_);

  if (itProject == worker.mapProjet_.end()) {
    return std::vector<uint>();
  }

  std::vector<uint> usersId = itProject->second.getConnected();
  auto it = std::find(usersId.begin(), usersId.end(), userId_);

  if (it != usersId.end()) {
    *it = usersId.back();
    usersId.pop_back();
  }
  return usersId;
}
ChangeRoleMessage::ChangeRoleMessage(sf::Packet &data_packet, uint userId) {
  userId_ = userId;
  data_packet >> target_ >> projectId_ >> role_;
}

void ChangeRoleMessage::process(Worker &worker) {
  bool success = worker.changeRole(target_, projectId_, role_);

  // Getting all connected users
  std::vector<uint> usersId;
  auto it = worker.mapProjet_.find(projectId_);
  if (it != worker.mapProjet_.end()) {
    usersId = it->second.getConnected();
  }

  // Building the group response
  std::unique_ptr<Reponse> rps;
  rps = std::make_unique<ReponseChangeRole>(usersId, target_, projectId_, role_,
                                            success);
  worker.pushNetwork(std::move(rps));
}

CreateLayerMessage::CreateLayerMessage(sf::Packet &data_packet,
                                       std::shared_ptr<Client> &client) {
  userId_ = client->id;
  data_packet >> projectId_ >> type_;
}

void CreateLayerMessage::process(Worker &worker) {
  auto liveProj = worker.mapProjet_.find(projectId_);

  if (liveProj == worker.mapProjet_.end()) {
    return;
  }

  if (liveProj->second.addCalque(userId_, type_)) {
    std::vector<uint> usersId = this->getUserLists(worker);
    std::unique_ptr<Reponse> rps;
    rps = std::make_unique<ReponseCreateLayer>(usersId, *this);
    worker.pushNetwork(std::move(rps));
  }
}

DeleteLayerMessage::DeleteLayerMessage(sf::Packet &data_packet,
                                       std::shared_ptr<Client> &client) {
  userId_ = client->id;
  data_packet >> projectId_ >> calqueId_;
}

void DeleteLayerMessage::process(Worker &worker) {
  auto liveProj = worker.mapProjet_.find(projectId_);

  if (liveProj == worker.mapProjet_.end()) {
    return;
  }

  if (liveProj->second.removeCalque(userId_, calqueId_)) {
    std::vector<uint> usersId = this->getUserLists(worker);
    std::unique_ptr<Reponse> rps;
    rps = std::make_unique<ReponseDeleteLayer>(usersId, *this);
    worker.pushNetwork(std::move(rps));
  }
}

RenameLayerMessage::RenameLayerMessage(sf::Packet &data_packet,
                                       std::shared_ptr<Client> &client) {
  userId_ = client->id;
  data_packet >> projectId_ >> calqueId_ >> name_;
}

void RenameLayerMessage::process(Worker &worker) {
  auto liveProj = worker.mapProjet_.find(projectId_);

  if (liveProj == worker.mapProjet_.end()) {
    return;
  }
  if (liveProj->second.renameCalque(userId_, calqueId_, name_)) {
    std::vector<uint> usersId = this->getUserLists(worker);
    std::unique_ptr<Reponse> rps;
    rps = std::make_unique<ReponseRenameLayer>(usersId, *this);
    worker.pushNetwork(std::move(rps));
  }
}

OrganizeLayerDownMessage::OrganizeLayerDownMessage(
    sf::Packet &data_packet, std::shared_ptr<Client> &client) {
  userId_ = client->id;
  data_packet >> projectId_ >> calqueId_;
}
void OrganizeLayerDownMessage::process(Worker &worker) {
  auto liveProj = worker.mapProjet_.find(projectId_);

  if (liveProj == worker.mapProjet_.end()) {
    return;
  }

  if (liveProj->second.moveCalqueDown(userId_, calqueId_)) {
    std::vector<uint> usersId = this->getUserLists(worker);
    std::unique_ptr<Reponse> rps;
    rps = std::make_unique<ReponseOrganizeLayerDown>(usersId, *this);
    worker.pushNetwork(std::move(rps));
  }
}

OrganizeLayerUpMessage::OrganizeLayerUpMessage(
    sf::Packet &data_packet, std::shared_ptr<Client> &client) {
  userId_ = client->id;
  data_packet >> projectId_ >> calqueId_;
}

void OrganizeLayerUpMessage::process(Worker &worker) {
  auto liveProj = worker.mapProjet_.find(projectId_);

  if (liveProj == worker.mapProjet_.end()) {
    return;
  }

  if (liveProj->second.moveCalqueUp(userId_, calqueId_)) {
    std::vector<uint> usersId = this->getUserLists(worker);
    std::unique_ptr<Reponse> rps;
    rps = std::make_unique<ReponseOrganizeLayerUp>(usersId, *this);
    worker.pushNetwork(std::move(rps));
  }
}

PutPixelsSquareMessage::PutPixelsSquareMessage(
    sf::Packet &data_packet, std::shared_ptr<Client> &client) {
  userId_ = client->id;
  data_packet >> projectId_ >> calqueId_ >> pos_.x >> pos_.y >> taille_ >>
      red_ >> green_ >> blue_ >> opa_;
}

void PutPixelsSquareMessage::process(Worker &worker) {
  auto liveProj = worker.mapProjet_.find(projectId_);

  if (liveProj == worker.mapProjet_.end()) {
    return;
  }

  if (liveProj->second.drawPixelRect(userId_, calqueId_, pos_.x, pos_.y,
                                     taille_, red_, green_, blue_, opa_)) {
    std::vector<uint> usersId = this->getUserLists(worker);
    std::unique_ptr<Reponse> rps;
    rps = std::make_unique<ReponsePutPixelsSquare>(usersId, *this);
    worker.pushNetwork(std::move(rps));
  }
}

PutPixelsCircleMessage::PutPixelsCircleMessage(
    sf::Packet &data_packet, std::shared_ptr<Client> &client) {
  userId_ = client->id;
  data_packet >> projectId_ >> calqueId_ >> pos_.x >> pos_.y >> taille_ >>
      red_ >> green_ >> blue_ >> opa_;
}

void PutPixelsCircleMessage::process(Worker &worker) {
  auto liveProj = worker.mapProjet_.find(projectId_);

  if (liveProj == worker.mapProjet_.end()) {
    return;
  }

  if (liveProj->second.drawPixelCircle(userId_, calqueId_, pos_.x, pos_.y,
                                       taille_, red_, green_, blue_, opa_)) {
    std::vector<uint> usersId = this->getUserLists(worker);
    std::unique_ptr<Reponse> rps;
    rps = std::make_unique<ReponsePutPixelsCircle>(usersId, *this);
    worker.pushNetwork(std::move(rps));
  }
}

PutPixelsDiamondMessage::PutPixelsDiamondMessage(
    sf::Packet &data_packet, std::shared_ptr<Client> &client) {
  userId_ = client->id;
  data_packet >> projectId_ >> calqueId_ >> pos_.x >> pos_.y >> hauteur_ >>
      largeur_ >> red_ >> green_ >> blue_ >> opa_;
}

void PutPixelsDiamondMessage::process(Worker &worker) {
  auto liveProj = worker.mapProjet_.find(projectId_);

  if (liveProj == worker.mapProjet_.end()) {
    return;
  }

  if (liveProj->second.drawPixelDiam(userId_, calqueId_, pos_.x, pos_.y,
                                     hauteur_, largeur_, red_, green_, blue_,
                                     opa_)) {
    std::vector<uint> usersId = this->getUserLists(worker);
    std::unique_ptr<Reponse> rps;
    rps = std::make_unique<ReponsePutPixelsDiamond>(usersId, *this);
    worker.pushNetwork(std::move(rps));
  }
}

ErasePixelsSquareMessage::ErasePixelsSquareMessage(
    sf::Packet &data_packet, std::shared_ptr<Client> &client) {
  userId_ = client->id;
  data_packet >> projectId_ >> calqueId_ >> pos_.x >> pos_.y >> taille_;
}

void ErasePixelsSquareMessage::process(Worker &worker) {
  auto liveProj = worker.mapProjet_.find(projectId_);

  if (liveProj == worker.mapProjet_.end()) {
    return;
  }

  if (liveProj->second.erasePixelRect(userId_, calqueId_, pos_.x, pos_.y,
                                      taille_)) {
    std::vector<uint> usersId = this->getUserLists(worker);
    std::unique_ptr<Reponse> rps;
    rps = std::make_unique<ReponseErasePixelsSquare>(usersId, *this);
    worker.pushNetwork(std::move(rps));
  }
}

ErasePixelsCircleMessage::ErasePixelsCircleMessage(
    sf::Packet &data_packet, std::shared_ptr<Client> &client) {
  userId_ = client->id;
  data_packet >> projectId_ >> calqueId_ >> pos_.x >> pos_.y >> taille_;
}

void ErasePixelsCircleMessage::process(Worker &worker) {
  auto liveProj = worker.mapProjet_.find(projectId_);

  if (liveProj == worker.mapProjet_.end()) {
    return;
  }

  if (liveProj->second.erasePixelCircle(userId_, calqueId_, pos_.x, pos_.y,
                                        taille_)) {
    std::vector<uint> usersId = this->getUserLists(worker);
    std::unique_ptr<Reponse> rps;
    rps = std::make_unique<ReponseErasePixelsCircle>(usersId, *this);
    worker.pushNetwork(std::move(rps));
  }
}

ErasePixelsDiamondMessage::ErasePixelsDiamondMessage(
    sf::Packet &data_packet, std::shared_ptr<Client> &client) {
  userId_ = client->id;
  data_packet >> projectId_ >> calqueId_ >> pos_.x >> pos_.y >> hauteur_ >>
      largeur_;
}

void ErasePixelsDiamondMessage::process(Worker &worker) {
  auto liveProj = worker.mapProjet_.find(projectId_);

  if (liveProj == worker.mapProjet_.end()) {
    return;
  }

  if (liveProj->second.erasePixelDiam(userId_, calqueId_, pos_.x, pos_.y,
                                      hauteur_, largeur_)) {
    std::vector<uint> usersId = this->getUserLists(worker);
    std::unique_ptr<Reponse> rps;
    rps = std::make_unique<ReponseErasePixelsDiamond>(usersId, *this);
    worker.pushNetwork(std::move(rps));
  }
}

PutSpriteMessage::PutSpriteMessage(sf::Packet &data_packet,
                                   std::shared_ptr<Client> &client) {
  userId_ = client->id;
  data_packet >> projectId_ >> calqueId_ >> asset_id_ >> pos_.x >> pos_.y >>
      taille_;
}
void PutSpriteMessage::process(Worker &worker) {
  auto liveProj = worker.mapProjet_.find(projectId_);

  if (liveProj == worker.mapProjet_.end()) {
    return;
  }

  if (liveProj->second.addSprite(userId_, calqueId_, asset_id_, pos_.x, pos_.y,
                                 taille_)) {
    std::vector<uint> usersId = this->getUserLists(worker);
    std::unique_ptr<Reponse> rps;

    rps = std::make_unique<ReponsePutSprite>(usersId, *this);
    worker.pushNetwork(std::move(rps));
  }
}

EraseSpriteMessage::EraseSpriteMessage(sf::Packet &data_packet,
                                       std::shared_ptr<Client> &client) {
  userId_ = client->id;
  data_packet >> projectId_ >> calqueId_ >> sprite_id_;
  qDebug() << projectId_ << " " << calqueId_ << " " << sprite_id_;
}

void EraseSpriteMessage::process(Worker &worker) {
  auto liveProj = worker.mapProjet_.find(projectId_);

  if (liveProj == worker.mapProjet_.end()) {
    return;
  }

  if (liveProj->second.removeSprite(userId_, calqueId_, sprite_id_)) {
    std::vector<uint> usersId = this->getUserLists(worker);
    std::unique_ptr<Reponse> rps;
    rps = std::make_unique<ReponseEraseSprite>(usersId, *this);
    worker.pushNetwork(std::move(rps));
  }
}

MoveSpriteMessage::MoveSpriteMessage(sf::Packet &data_packet,
                                     std::shared_ptr<Client> &client) {
  userId_ = client->id;
  uint count;
  data_packet >> projectId_ >> calqueId_ >> count >> x_ >> y_;
  sprite_ids_.resize(count);
  for (uint i = 0; i < count; ++i) {
    data_packet >> sprite_ids_[i];
  }
}

void MoveSpriteMessage::process(Worker &worker) {
  auto liveProj = worker.mapProjet_.find(projectId_);
  if (liveProj == worker.mapProjet_.end()) {
    std::cout << "Projet non trouvé pour MoveSpriteMessage" << std::endl;
    return;
  }
  if (liveProj->second.moveSprite(userId_, calqueId_, std::move(sprite_ids_),x_,y_)){
  std::vector<uint> usersId = this->getUserLists(worker);
  std::unique_ptr<Reponse> rps;

  rps = std::make_unique<ReponseMoveSprite>(usersId, *this);
  worker.pushNetwork(std::move(rps));
  }
}

ResizeSpriteMessage::ResizeSpriteMessage(sf::Packet &data_packet,
                                         std::shared_ptr<Client> &client) {
  userId_ = client->id;
  uint count;
  data_packet >> projectId_ >> calqueId_ >> count >> scale_;
  sprite_ids_.resize(count);
  x_.resize(count);
  y_.resize(count);
  for (uint i = 0; i < count; ++i) {
    data_packet >> sprite_ids_[i] >> x_[i] >> y_[i];
  }
}

void ResizeSpriteMessage::process(Worker &worker) {
  auto liveProj = worker.mapProjet_.find(projectId_);
  if (liveProj == worker.mapProjet_.end()) {
    return;
  }
  if (liveProj->second.resizeSprite(userId_, calqueId_, sprite_ids_, scale_, x_,y_)){
  std::vector<uint> usersId = this->getUserLists(worker);
  std::unique_ptr<Reponse> rps;

  rps = std::make_unique<ReponseResizeSprite>(usersId, *this);
  worker.pushNetwork(std::move(rps));
  }
}

RotateSpriteMessage::RotateSpriteMessage(sf::Packet &data_packet,
                                         std::shared_ptr<Client> &client) {
  userId_ = client->id;
  uint count;
  data_packet >> projectId_ >> calqueId_ >> count >> angle_;
  sprite_ids_.resize(count);
  x_.resize(count);
  y_.resize(count);
  for (uint i = 0; i < count; ++i) {
    data_packet >> sprite_ids_[i] >> x_[i] >> y_[i];
  }
}

void RotateSpriteMessage::process(Worker &worker) {
  auto liveProj = worker.mapProjet_.find(projectId_);
  if (liveProj == worker.mapProjet_.end()) {
    return;
  }
  if (liveProj->second.rotateSprite(userId_, calqueId_, sprite_ids_, angle_, x_, y_)){
  std::vector<uint> usersId = this->getUserLists(worker);
  std::unique_ptr<Reponse> rps;

  rps = std::make_unique<ReponseRotateSprite>(usersId, *this);
  worker.pushNetwork(std::move(rps));
  }
}

MoveLayerMessage::MoveLayerMessage(sf::Packet &data_packet,
                                   std::shared_ptr<Client> &client) {
  userId_ = client->id;
  data_packet >> projectId_ >> calqueId_ >> deltaX_ >> deltaY_;
}

void MoveLayerMessage::process(Worker &worker) {
  auto liveProj = worker.mapProjet_.find(projectId_);
  if (liveProj == worker.mapProjet_.end()) {
    return;
  }
  if (liveProj->second.shiftCalque(userId_, calqueId_, deltaX_, deltaY_)) {
    std::vector<uint> usersId = this->getUserLists(worker);
    std::unique_ptr<Reponse> rps;

    rps = std::make_unique<ReponseMoveLayer>(usersId, *this);
    worker.pushNetwork(std::move(rps));
  }
}

GenerateTokenMessage::GenerateTokenMessage(sf::Packet &dataPacket,
                                           std::shared_ptr<Client> &client) {
  dataPacket >> role >> projectId_;
  userId_ = client->id;
}

void GenerateTokenMessage::process(Worker &worker) {
  std::string token = worker.generateShareToken(role, projectId_);
  std::cout << "TOKENNN" << token << std::endl;
  std::unique_ptr<Reponse> rps;
  rps = std::make_unique<ReponseGenerateToken>(userId_, token);
  worker.pushNetwork(std::move(rps));
}

CheckTokenMessage::CheckTokenMessage(sf::Packet &dataPacket,
                                     std::shared_ptr<Client> &client) {
  dataPacket >> token;
  userId_ = client->id;
}

void CheckTokenMessage::process(Worker &worker) {
  bool sucess = worker.checkShareToken(userId_, token);
  std::cout << "success or not ?" << sucess << std::endl;
  std::unique_ptr<Reponse> rps;
  rps = std::make_unique<ReponseJoinProject>(userId_, sucess);
  worker.pushNetwork(std::move(rps));
}

DisconnectMessage::DisconnectMessage(std::shared_ptr<Client> &client) {
  userId_ = client->id;
  projectId_ = client->projectId;
  pseudo_ = client->pseudo;
}

void DisconnectMessage::process(Worker &worker) {
  std::unique_ptr<Reponse> rps;
  rps = std::make_unique<ReponseDeconnection>(userId_);
  worker.pushNetwork(std::move(rps));

  auto itProject = worker.mapProjet_.find(projectId_);

  if (itProject == worker.mapProjet_.end()) {
    return;
  }

  if (itProject->second.removeConnection(userId_)) {
    createSystemNotification(worker, projectId_, pseudo_, userId_, typeNotification::DECONNEXION);
  }

  if (itProject->second.isEmpty()) {
    std::unique_ptr<SaveTask> savetsk;
    savetsk = std::make_unique<SaveTask>(worker.mapProjet_.at(projectId_),
                                         projectId_);
    worker.pushSave(std::move(savetsk));
    // Message de sauvegarde de projet
    worker.mapProjet_.erase(projectId_);
  }
}

ChatMessage::ChatMessage(sf::Packet &dataPacket,
                         std::shared_ptr<Client> &client) {
  userId_ = client->id;
  pseudo_ = client->pseudo;
  projectId_ = client->projectId;
  dataPacket >> message_;
}

void ChatMessage::process(Worker &worker) {
  auto itProject = worker.mapProjet_.find(projectId_);

  if (itProject == worker.mapProjet_.end()) {
    return;
  }
  std::shared_ptr<MessageChat> messageChat =
      std::make_shared<UserMessage>(pseudo_, userId_, message_);

  Date date = messageChat->getDate();
  if (itProject->second.addMessageChat(userId_, messageChat)) {
    std::vector<uint> usersId = itProject->second.getConnected();
    std::unique_ptr<Reponse> rps;

    min_ = date.min_;
    hour_ = date.hour_;
    day_ = date.day_;
    month_ = date.month_;
    year_ = date.year_;
    rps = std::make_unique<ReponseChat>(usersId, *this);
    worker.pushNetwork(std::move(rps));
  }
}

void createSystemNotification(Worker &worker, uint projectId, std::string &pseudo, uint userId, typeNotification type) {
  auto itProject = worker.mapProjet_.find(projectId);

  if (itProject == worker.mapProjet_.end()) {
    return;
    std::cout << "impossible de créer une notification de système" << std::endl;
  }

  std::shared_ptr<SystemNotification> messageChat =
      std::make_shared<SystemNotification>(pseudo, userId, type);
  

  if (itProject->second.addMessageChat(userId, messageChat)) {
    std::vector<uint> usersId = itProject->second.getConnected();

    std::cout << "création d'une notification de système pour l'utilisateur " << userId << std::endl;

    std::unique_ptr<Reponse> rps;
    rps = std::make_unique<ReponseChatSystem>(usersId, *messageChat);
    worker.pushNetwork(std::move(rps));
  }
}


HomeMessage::HomeMessage(std::shared_ptr<Client>& client) {
    userId_ = client->id;
    projectId_ = client->projectId;
    pseudo_ = client->pseudo;
}

void HomeMessage::process(Worker& worker){
  auto itProject = worker.mapProjet_.find(projectId_);
    
    if (itProject == worker.mapProjet_.end()) {
        return; 
    }
    
    if (itProject->second.removeConnection(userId_)) {
        createSystemNotification(worker, projectId_, pseudo_, userId_, typeNotification::DECONNEXION);
    }

    if (itProject->second.isEmpty()) {
        std::unique_ptr<SaveTask> savetsk;
        savetsk = std::make_unique<SaveTask>(worker.mapProjet_.at(projectId_), projectId_);
        worker.pushSave(std::move(savetsk));
        //Message de sauvegarde de projet
        worker.mapProjet_.erase(projectId_);
    }
}

AddSpriteMessage::AddSpriteMessage(sf::Packet &dataPacket,std::shared_ptr<Client>& client){
  userId_ = client->id;
  projectId_ = client->projectId;
  pseudo_ = client->pseudo;
  dataPacket >> sprite_;
}

void AddSpriteMessage::process(Worker &worker){
  auto itProject = worker.mapProjet_.find(projectId_);

  if (itProject == worker.mapProjet_.end()) {
    return;
    std::cout << "impossible de créer une notification de système" << std::endl;
  }
  std::vector<uint> usersId = itProject->second.getConnected();

  std::unique_ptr<Reponse> rps;
  rps = std::make_unique<ReponseAddSprite>(usersId, *this);
  worker.pushNetwork(std::move(rps));
}


std::unique_ptr<IMessage> MessageFactory(sf::Packet &data_packet,
                                         std::shared_ptr<Client> &c) {
  uint8_t typeRaw;
  if (!(data_packet >> typeRaw))
    return nullptr;
  MsgProtocole type = static_cast<MsgProtocole>(typeRaw);
  std::cout << "[From client " << c->id << "]:" << to_string(type) << std::endl;
  switch (type) {
  case MsgProtocole::AUTH_LOGIN_REQ:
    return std::make_unique<LoginMessage>(data_packet, std::move(c));
  case MsgProtocole::AUTH_REGISTER_REQ:
    return std::make_unique<RegisterMessage>(data_packet, std::move(c));
  case MsgProtocole::LOB_CREATE_PROJECT_REQ:
    return std::make_unique<CreateProjectMessage>(data_packet, c);

  case MsgProtocole::LOB_PROJECT_LIST_REQ:
    return std::make_unique<GetProjectsListMessage>(data_packet, c);

  case MsgProtocole::LOB_DEL_PROJECT_REQ:
    return std::make_unique<DeleteProjectMessage>(data_packet, c);

  case MsgProtocole::LOB_GET_PROJECT_DATA_REQ:
    return std::make_unique<GetProjectDataMessage>(data_packet, c);

  case MsgProtocole::LOB_RENAME_PROJECT_REQ:
    return std::make_unique<RenameProjectMessage>(data_packet, c);

  case MsgProtocole::LOB_DUPLICATE_PROJECT_REQ:
    return std::make_unique<DuplicateProjectMessage>(data_packet, c);

  case MsgProtocole::PROJ_GET_MEMBERS_REQ:
    return std::make_unique<GetMemberMessage>(data_packet, c->id);

  case MsgProtocole::LOB_SHARE_PROJECT_REQ:
    return std::make_unique<GenerateTokenMessage>(data_packet, c);

  case MsgProtocole::LOB_JOIN_PROJECT_REQ:
    return std::make_unique<CheckTokenMessage>(data_packet, c);

  case MsgProtocole::LOB_EXPORT_NATIVE_PROJECT_REQ:
    return std::make_unique<ExportNativeMessage>(data_packet, c);

  case MsgProtocole::LOB_IMPORT_PROJECT_REQ:
    return std::make_unique<ImportProjectMessage>(data_packet, c);

  case MsgProtocole::MAP_CREATE_LAYER_REQ:
    return std::make_unique<CreateLayerMessage>(data_packet, c);

  case MsgProtocole::MAP_REMOVE_LAYER_REQ:
    return std::make_unique<DeleteLayerMessage>(data_packet, c);

  case MsgProtocole::MAP_RENAME_LAYER_REQ:
    return std::make_unique<RenameLayerMessage>(data_packet, c);

  case MsgProtocole::MAP_ORGANIZE_LAYER_UP_REQ:
    return std::make_unique<OrganizeLayerUpMessage>(data_packet, c);

  case MsgProtocole::MAP_ORGANIZE_LAYER_DOWN_REQ:
    return std::make_unique<OrganizeLayerDownMessage>(data_packet, c);

  case MsgProtocole::MAP_PUT_PIXELS_SQUARE_REQ:
    return std::make_unique<PutPixelsSquareMessage>(data_packet, c);

  case MsgProtocole::MAP_PUT_PIXELS_CIRCLE_REQ:
    return std::make_unique<PutPixelsCircleMessage>(data_packet, c);

  case MsgProtocole::MAP_PUT_PIXELS_DIAM_REQ:
    return std::make_unique<PutPixelsDiamondMessage>(data_packet, c);

  case MsgProtocole::MAP_ERASE_PIXELS_SQUARE_REQ:
    return std::make_unique<ErasePixelsSquareMessage>(data_packet, c);

  case MsgProtocole::MAP_ERASE_PIXELS_CIRCLE_REQ:
    return std::make_unique<ErasePixelsCircleMessage>(data_packet, c);

  case MsgProtocole::MAP_ERASE_PIXELS_DIAM_REQ:
    return std::make_unique<ErasePixelsDiamondMessage>(data_packet, c);

  case MsgProtocole::MAP_MOV_SPRITE_REQ:
    return std::make_unique<MoveSpriteMessage>(data_packet, c);

  case MsgProtocole::MAP_RESIZE_SPRITE_REQ:
    return std::make_unique<ResizeSpriteMessage>(data_packet, c);

  case MsgProtocole::MAP_ROTATE_SPRITE_REQ:
    return std::make_unique<RotateSpriteMessage>(data_packet, c);

  case MsgProtocole::MAP_MOV_LAYER_REQ:
    return std::make_unique<MoveLayerMessage>(data_packet, c);

  case MsgProtocole::MAP_PUT_SPRITE_REQ:
    return std::make_unique<PutSpriteMessage>(data_packet, c);

  case MsgProtocole::MAP_ERASE_SPRITE_REQ:
    return std::make_unique<EraseSpriteMessage>(data_packet, c);

  case MsgProtocole::PROJ_CHANGE_ROLE_REQ:
    return std::make_unique<ChangeRoleMessage>(data_packet, c->id);

  case MsgProtocole::PROJ_LEAVE_PROJ_REQ:
    return std::make_unique<LeaveProjectMessage>(data_packet, c->id);

  case MsgProtocole::CHAT_MESSAGE_REQ:
    return std::make_unique<ChatMessage>(data_packet, c);

  case MsgProtocole::LOB_HOME_REQ:
    return std::make_unique<HomeMessage>(c);
    
  case MsgProtocole::PROJ_KICK_USER_REQ:
    return std::make_unique<KickUserMessage>(data_packet, c->id);
  case MsgProtocole::MAP_ADD_SPRITE_REQ:
    return std::make_unique<AddSpriteMessage>(data_packet, c);
  default:
    std::cout << "pas de message" << std::endl;
    return nullptr;
  }
}
