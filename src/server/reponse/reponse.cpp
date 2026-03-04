#include "reponse.hpp"
#include "../protocol.hpp"
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

ReponseProjectData::ReponseProjectData(long long userId) : ReponseSolo(userId) {/*remplir un jour lol*/}


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


ReponseGroupe::ReponseGroupe(std::vector<long long> usersId) : usersId_(std::move(usersId)) {}

void ReponseGroupe::envoyer(ServerNetworkManager& servManager) {
    for (auto Id : usersId_) {
        auto client = servManager.map_.find(Id);
        if (client != servManager.map_.end()) {
            client->second->sock->send(dataPacket_);
        }
    } 
}



