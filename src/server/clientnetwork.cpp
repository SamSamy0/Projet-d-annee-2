#include "clientnetwork.hpp"



std::deque<ServerRep>& ClientNetworkManager::getQueuRep(){
    return reponse;
}


bool ClientNetworkManager::connect(){
    socket.setBlocking(false);

    if (socket.connect({127,0,0,1},5000) == sf::Socket::Done){
        return true;
    }else return false;
}


void ClientNetworkManager::getReponse(){
    ServerRep rep;
    auto packet = std::make_unique<sf::Packet>();

    if (socket.receive(*packet) == sf::Socket::Done){
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


void ClientNetworkManager::createProject(std::string role, float size, float scale){
    sf::Packet packet;
    MsgProtocole msg = MsgProtocole::LOB_CREATE_PROJECT_REQ;
    
    packet << static_cast<uint8_t>(msg);
    packet << role << size << scale ;

    socket.send(packet);

}




