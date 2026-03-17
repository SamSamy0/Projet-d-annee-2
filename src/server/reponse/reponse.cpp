#include "reponse.hpp"
#include "../../common/protocol.hpp"
#include "../servernetwork.hpp"


ReponseSolo::ReponseSolo(long long id) : userId_(id) {}

void ReponseSolo::envoyer(ServerNetworkManager& servManage) {
    auto client = servManage.map_.find(userId_);
    if (client != servManage.map_.end()) {
        client->second->sock->send(dataPacket_);
    }
}

ReponseAuth::ReponseAuth(std::shared_ptr<Client> client, long long id)
: ReponseSolo(id), client_(std::move(client)) {
    
    dataPacket_ << static_cast<std::uint8_t>(MsgProtocole::AUTH_RESULT);
    if (id == -1) {
        dataPacket_ << static_cast<std::uint8_t>(0);
    } else {
        dataPacket_ << static_cast<std::uint8_t>(1);
        client_->id = id;
    }
}

void ReponseAuth::envoyer(ServerNetworkManager& servManage) {
    client_->sock->send(dataPacket_);
    
    if (userId_ != 0) {
        servManage.map_[userId_] = std::move(client_);
    }
}

ReponseRenameProject::ReponseRenameProject(long long userId, int projectId, std::string newName, bool success): ReponseSolo(userId){
    dataPacket_ <<static_cast<std::uint8_t> (MsgProtocole::LOB_RENAME_PROJECT_REP);
    dataPacket_ << static_cast<std::uint8_t>(success ?1:0);
    dataPacket_ <<static_cast<std::uint32_t>(projectId);
    dataPacket_<<newName;;
}

ReponseDuplicateProject::ReponseDuplicateProject(long long userId, int projectId, std::string newName, bool success): ReponseSolo(userId){
    dataPacket_ <<static_cast<std::uint8_t> (MsgProtocole::LOB_DUPLICATE_PROJECT_REP);
    dataPacket_ << static_cast<std::uint8_t>(success ?1:0);
    dataPacket_ <<static_cast<std::uint32_t>(projectId);
    dataPacket_<<newName;;
    
}

ReponseProjectData::ReponseProjectData(long long userId, QByteArray& jsonData) : ReponseSolo(userId) {
    dataPacket_<<static_cast<std::uint8_t> (MsgProtocole::LOB_GET_PROJECT_DATA_REP);

    QByteArray jsonCompresse = qCompress(jsonData, 9);
    dataPacket_ << static_cast<std::uint32_t>(jsonCompresse.size());
    dataPacket_.append(jsonCompresse.constData(), jsonCompresse.size());
}

ReponseUsersProjects::ReponseUsersProjects(long long userId, std::vector<ProjectEntry>& projects) 
: ReponseSolo(userId) {
    dataPacket_ << static_cast<std::uint8_t>(MsgProtocole::LOB_PROJECT_LIST_REP);

    dataPacket_ << static_cast<std::uint32_t>(projects.size());

    for (const auto& entry : projects) {
        dataPacket_ << static_cast<uint32_t>(entry.projectId) << entry.name << entry.role;
    }
}

ReponseCreateProject::ReponseCreateProject(long long userId, int projectId) : ReponseSolo(userId) {
    dataPacket_ << static_cast<std::uint8_t>(MsgProtocole::LOB_CREATE_PROJECT_REP);

    dataPacket_ << projectId;
}

ReponseGroupe::ReponseGroupe(std::vector<long long> usersId) : usersId_(std::move(usersId)) {}


void ReponseGroupe::envoyer(ServerNetworkManager& servManager) {
    for (auto Id : usersId_) {
        auto client = servManager.map_.find(Id);
        if (client != servManager.map_.end()) {
            client->second->sock->send(dataPacket_);
        }
    } 
}



ReponsePutPixelsCircle::ReponsePutPixelsCircle(std::vector<long long> usersId, PutPixelsCircleMessage& mess) : ReponseGroupe(usersId) {
    dataPacket_ << static_cast<std::uint8_t>(MsgProtocole::MAP_PUT_PIXELS_CIRCLE_REP);

    dataPacket_ << mess.projectId_ << mess.calqueId_ <<  mess.pos_.x << mess.pos_.y; 
    dataPacket_ << mess.red_ << mess.green_ << mess.blue_ << mess.opa_ << mess.taille_;
}

ReponsePutPixelsCarre::ReponsePutPixelsCarre(std::vector<long long> usersId, PutPixelsCarreMessage& mess) : ReponseGroupe(usersId){
    dataPacket_ << static_cast<std::uint8_t>(MsgProtocole::MAP_PUT_PIXELS_CARRE_REP);

    dataPacket_ << mess.projectId_ << mess.calqueId_ <<  mess.pos_.x << mess.pos_.y; 
    dataPacket_ << mess.red_ << mess.green_ << mess.blue_ << mess.opa_ << mess.taille_;
}

ReponsePutPixelsLosange::ReponsePutPixelsLosange(std::vector<long long> usersId, PutPixelsLosangeMessage& mess) : ReponseGroupe(usersId){
    dataPacket_ << static_cast<std::uint8_t>(MsgProtocole::MAP_PUT_PIXELS_LOSAN_REP);

    dataPacket_ << mess.projectId_ << mess.calqueId_ <<  mess.pos_.x << mess.pos_.y; 
    dataPacket_ << mess.red_ << mess.green_ << mess.blue_ << mess.opa_ << mess.hauteur_ << mess.largeur_;
}


ReponseErasePixelsCircle::ReponseErasePixelsCircle(std::vector<long long> usersId, ErasePixelsCircleMessage& mess) : ReponseGroupe(usersId) {
    dataPacket_ << static_cast<std::uint8_t>(MsgProtocole::MAP_ERASER_CIRCLE_REP);

    dataPacket_ << mess.projectId_ << mess.calqueId_ <<  mess.pos_.x << mess.pos_.y; 
    dataPacket_ << mess.taille_;
}

ReponseErasePixelsCarre::ReponseErasePixelsCarre(std::vector<long long> usersId, ErasePixelsCarreMessage& mess) : ReponseGroupe(usersId){
    dataPacket_ << static_cast<std::uint8_t>(MsgProtocole::MAP_ERASER_CARRE_REP);

    dataPacket_ << mess.projectId_ << mess.calqueId_ <<  mess.pos_.x << mess.pos_.y; 
    dataPacket_ << mess.taille_;
}

ReponseErasePixelsLosange::ReponseErasePixelsLosange(std::vector<long long> usersId, ErasePixelsLosangeMessage& mess) : ReponseGroupe(usersId){
    dataPacket_ << static_cast<std::uint8_t>(MsgProtocole::MAP_ERASER_LOSAN_REP);

    dataPacket_ << mess.projectId_ << mess.calqueId_ <<  mess.pos_.x << mess.pos_.y; 
    dataPacket_ << mess.hauteur_ << mess.largeur_;
}