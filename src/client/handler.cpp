#include "handler.hpp"
#include "clientnetwork.hpp"


ClientHandler::ClientHandler(ClientNetworkManager& client_manager, ReceiverInWindow w)
: manager_(&client_manager),handleWindow_(&w){}


void ClientHandler::trateEventQueu(){
    while ((*manager_).hasEvent()){
        ServerEvent event = (*manager_).popEvent();
        process(event);
    }
}


void ClientHandler::process(ServerEvent& event){
    switch(event.message_type_){
        case MsgProtocole::AUTH_RESULT:
            uint8_t accept;
            *(event.data_packet_) >> accept;
            handleWindow_->switchConnectState(accept);
            break;
        case MsgProtocole::LOB_PROJECT_LIST_REP:
            break;
    }        
}





