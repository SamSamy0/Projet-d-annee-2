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
        std::cout << "le map est mis à jour" << std::endl;
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
    std::cout << "MESSAGE CREER" << std::endl;
}

ReponseUsersProjects::ReponseUsersProjects(long long userId, std::vector<ProjectEntry>& projects) 
: ReponseSolo(userId) {
    std::cout << "je fais le pack de projet" << std::endl;
    dataPacket_ << static_cast<std::uint8_t>(MsgProtocole::LOB_PROJECT_LIST_REP);

    dataPacket_ << static_cast<std::uint32_t>(projects.size());
    std::cout << static_cast<int>(projects.size()) << std::endl;

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


