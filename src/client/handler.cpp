#include "handler.hpp"
#include "clientnetwork.hpp"
#include <iostream>


ClientHandler::ClientHandler(ClientNetworkManager& client_manager, ReceiverInWindow w)
: manager_(&client_manager),handleWindow_(&w){}


void ClientHandler::processEventQueu(){
    while ((*manager_).hasEvent()){
        std::cout <<"####################  evenement en cours de traitement  ########################" << std::endl;
        ServerEvent event = (*manager_).popEvent();
        process(event);
    }
}


void ClientHandler::process(ServerEvent& event){
    std::cout <<"####################  process event  ########################" << std::endl;
    std::cout << static_cast<int>(event.message_type_) << std::endl;
    switch(event.message_type_){
        case MsgProtocole::AUTH_RESULT:
        std::cout <<"####################  connexion traitement  ########################" << std::endl;
            uint8_t accept;
            *(event.data_packet_) >> accept;
            handleWindow_->switchConnectState(accept);
            break;
        case MsgProtocole::LOB_PROJECT_LIST_REP:
            break;
    }        
}





