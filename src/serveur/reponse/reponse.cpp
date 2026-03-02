#include "reponse.hpp"
#include "protocol.hpp"


ReponseSolo::ReponseSolo(long long id) : userId_(id) {}

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

void ReponseAuth::envoyer() {

}

ReponseProjectData::ReponseProjectData(long long userId) : ReponseSolo(userId) {/*remplir un jour lol*/}

void ReponseProjectData::envoyer() {/*remplir un jour lol*/}

ReponseUsersProjects::ReponseUsersProjects(long long userId, std::vector<ProjectEntry>& projects) 
: ReponseSolo(userId) {
    dataPacket_ << static_cast<std::uint8_t>(MsgProtocole::LOB_GET_MY_PROJECTS_DATA_REP);

    dataPacket_ << static_cast<std::uint32_t>(projects.size());

    for (const auto& entry : projects) {
        dataPacket_ << static_cast<uint64_t>(entry.projectId) << entry.name << entry.role;
    }
}

void ReponseUsersProjects::envoyer() {
    //à faire un jour
}



ReponseGroupe::ReponseGroupe(std::vector<long long> usersId) : usersId_(std::move(usersId)) {}

