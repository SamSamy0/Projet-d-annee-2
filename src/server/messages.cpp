#include "messages.hpp"
#include "protocol.hpp"
#include "worker.hpp"
#include "reponsequeue.hpp"
#include <utility>
#include <iostream>

 


LoginMessage::LoginMessage(std::shared_ptr<sf::Packet> data_packet, std::shared_ptr<Client> c): Message(c){
    *data_packet >> pseudo >> password;
}


void LoginMessage::process(Worker& worker){
    client->id = worker.verifyLogin(this->pseudo, this->password);

    //création de la réponse
    auto rps = std::make_shared<Reponse>();
    rps->client = this->client;
    rps->id_list.push_back(client->id);
    std::cout << rps->id_list[0] << std::endl;

    rps->message_type = MsgProtocole::AUTH_RESULT;

    rps->packet = std::make_shared<sf::Packet>();
    *rps->packet << static_cast<std::uint8_t>(MsgProtocole::AUTH_RESULT);
    if (client->id == -1) {
        *rps->packet << static_cast<std::uint8_t>(0);
    } else {
        *rps->packet << static_cast<std::uint8_t>(1);
    }

    //mise sur la liste des réponses
    worker.rep_queue.push(std::move(rps));
}



RegisterMessage::RegisterMessage(std::shared_ptr<sf::Packet> data_packet, std::shared_ptr<Client> c): Message(c){
    *data_packet >> pseudo >> password;
}


void RegisterMessage::process(Worker& worker) {
    client->id = worker.addUser(this->pseudo, this->password);
    std::cout << client->id << std::endl;
    

    //création de la réponse
    auto rps = std::make_shared<Reponse>();
    rps->client = this->client;
    rps->id_list.push_back(client->id);
    std::cout << rps->id_list[0] << std::endl;

    rps->message_type = MsgProtocole::AUTH_RESULT;

    rps->packet = std::make_shared<sf::Packet>();
    *rps->packet << static_cast<std::uint8_t>(MsgProtocole::AUTH_RESULT);
    if (client->id == -1) {
        *rps->packet << static_cast<std::uint8_t>(0);
    } else {
        *rps->packet << static_cast<std::uint8_t>(1);
    }

    //mise sur la liste des réponses
    std::cout << "j'essaie de push" << std::endl;
    worker.rep_queue.push(std::move(rps));
    std::cout << "j'ai réussi" << std::endl;

}


CreateProjectMessage::CreateProjectMessage(std::shared_ptr<sf::Packet> data_packet, std::shared_ptr<Client> c): Message(c){
    *data_packet >> nomProjet >> size.x >> size.y >> scale;
}


void CreateProjectMessage::process(Worker& worker) {
    long long id_proj = worker.db_Manager.addProject("test", this->client->id);
    worker.proj_Manager.createProjectJson(id_proj, "test", size.x, size.y, scale);

}


GetProjectDataMessage::GetProjectDataMessage(std::shared_ptr<sf::Packet> data_packet, std::shared_ptr<Client> c): Message(c) {
}


void GetProjectDataMessage::process(Worker& worker) {
    //à faire un jour
}

GetUsersProjectsMessage::GetUsersProjectsMessage(std::shared_ptr<sf::Packet> data_packet, std::shared_ptr<Client> c): Message(c) {
    *data_packet >> userdId_;

}

void GetUsersProjectsMessage::process(Worker& worker) {
    std::vector<ProjectEntry> projects = worker.db_Manager.getAllProjects();

    auto rps = std::make_shared<Reponse>();
    rps->client = this->client;
    rps->message_type = MsgProtocole::LOB_GET_MY_PROJECTS_DATA_REP;

    rps->packet = std::make_shared<sf::Packet>();
    *rps->packet << static_cast<std::uint8_t>(MsgProtocole::LOB_GET_MY_PROJECTS_DATA_REP);

    *rps->packet << static_cast<std::uint32_t>(projects.size());

    for (const auto& entry : projects) {
        *rps->packet << static_cast<std::uint32_t>(projects.size());
        *rps->packet << entry.name;
        *rps->packet << entry.role;
    }

    worker.rep_queue.push(std::move(rps));
}


std::shared_ptr<Message> MessageFactory(std::shared_ptr<sf::Packet> data_packet, std::shared_ptr<Client> c) {
    uint8_t typeRaw;
    // On lit le type depuis le shared_ptr (en le déréférençant)
    if (!(*data_packet >> typeRaw)) return nullptr;

    MsgProtocole type = static_cast<MsgProtocole>(typeRaw);

    switch (type) {
        case MsgProtocole::AUTH_LOGIN_REQ:
            // Ici, data_packet et c sont déjà des shared_ptr. Pas d'astérisque ici !
            return std::make_shared<LoginMessage>(data_packet, c);

        case MsgProtocole::AUTH_REGISTER_REQ:
            return std::make_shared<RegisterMessage>(data_packet, c);

        case MsgProtocole::LOB_CREATE_PROJECT_REQ:
            return std::make_shared<CreateProjectMessage>(data_packet, c);

        default:
            return nullptr;
    }
}



