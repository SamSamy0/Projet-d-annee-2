#include "clientnetwork.hpp"



std::deque<ServerEvent>& ClientNetworkManager::getQueuEvent(){
    return reponse;
}


bool ClientNetworkManager::connect(){
    socket.setBlocking(false);

    if (socket.connect({127,0,0,1},5000) == sf::Socket::Status::Done){
        return true;
    }else return false;
}

void ClientNetworkManager::getEvent(){
    ServerEvent rep;
    auto packet = std::make_unique<sf::Packet>();

    if (socket.receive(*packet) == sf::Socket::Status::Done){
        uint8_t type_mess;
        *packet >> type_mess;
        
        rep.message_type = static_cast<MsgProtocole>(type_mess);
        rep.data_packet = std::move(packet);

        reponse.push_back(std::move(rep));
            
    }
}


void ClientNetworkManager::login(std::string pseudo,std::string password){
    sf::Packet packet;
    MsgProtocole msg = MsgProtocole::AUTH_LOGIN_REQ;
    
    packet << static_cast<uint8_t>(msg);
    packet << pseudo << password ;

    socket.send(packet);
}


void ClientNetworkManager::askRegister(std::string pseudo, std::string password){
    sf::Packet packet;
    MsgProtocole msg = MsgProtocole::AUTH_REGISTER_REQ;
    
    packet << static_cast<uint8_t>(msg);
    packet << pseudo << password ;

    socket.send(packet);
}


void ClientNetworkManager::createProject(std::string nomProjet, sf::Vector2u size, float scale){
    sf::Packet packet;
    MsgProtocole msg = MsgProtocole::LOB_CREATE_PROJECT_REQ;
    
    packet << static_cast<uint8_t>(msg);
    packet << nomProjet << size.x << size.y << scale ;

    socket.send(packet);

}


void ClientNetworkManager::getProjectData(int project_id) {

}



void ClientNetworkManager::getUsersProjects(long long userId) {
    sf::Packet packet;
    MsgProtocole msg = MsgProtocole::LOB_GET_MY_PROJECTS_DATA_REQ;

    packet << static_cast<uint8_t>(msg);
    packet << static_cast<std::int64_t>(userId);
    
    socket.send(packet);
}

