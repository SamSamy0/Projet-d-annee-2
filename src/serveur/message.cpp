#pragma once
#include "message.hpp"
#include "worker.hpp"
#include "protocol.hpp"



LoginMessage::LoginMessage(sf::Packet& dataPacket) {
    dataPacket >> pseudo_ >> password_;
}


void LoginMessage::process(Worker& worker){
    long long id = worker.verifyLogin(this->pseudo_, this->password_);
}



RegisterMessage::RegisterMessage(sf::Packet& dataPacket) {
    dataPacket >> pseudo_ >> password_;
}


void RegisterMessage::process(Worker& worker) {
    long long id = worker.addUser(this->pseudo_, this->password_);
}


CreateProjectMessage::CreateProjectMessage(sf::Packet& data_packet, long long userId) {
    data_packet >> nomProjet_ >> size_.x >> size_.y >> scale_;
    userId_ = userId;
}


void CreateProjectMessage::process(Worker& worker) {
    long long idProj = worker.addProjectSQL("test", userId_);
    worker.createProjectJson(idProj, "test", size_.x, size_.y, scale_);
}


GetProjectDataMessage::GetProjectDataMessage(sf::Packet& data_packet, long long userId) {
}


void GetProjectDataMessage::process(Worker& worker) {
}

GetUsersProjectsMessage::GetUsersProjectsMessage(sf::Packet& data_packet, long long userId) {
    userId_ = userId;
}

void GetUsersProjectsMessage::process(Worker& worker) {
    std::vector<ProjectEntry> projects = worker.getAllProjects();
    std::cout << static_cast<int>(projects.size())<<std::endl;
}


std::unique_ptr<IMessage> MessageFactory(sf::Packet& data_packet, long long id) {
    uint8_t typeRaw;
    if (!(data_packet >> typeRaw)) return nullptr;

    MsgProtocole type = static_cast<MsgProtocole>(typeRaw);

    switch (type) {
        case MsgProtocole::LOB_CREATE_PROJECT_REQ:
            return std::make_unique<CreateProjectMessage>(data_packet, id);

        case MsgProtocole::LOB_GET_MY_PROJECTS_DATA_REP:
            return std::make_unique<GetUsersProjectsMessage>(data_packet, id);
            
        default:
            return nullptr;
    }
}