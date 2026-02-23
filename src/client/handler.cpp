#include "handler.hpp"
#include "clientnetwork.hpp"


ClientEventHandler::ClientEventHandler(ClientNetworkManager& client_manager, HandleRepInWindow w)
: manager_(&client_manager),handleWindow_(&w){}

void ClientEventHandler::trateEventQueu(){
    while ((*manager_).hasEvent()){
        ServerEvent event = (*manager_).popEvent();
        process(event);
    }
}

void ClientEventHandler::process(ServerEvent& event){
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




HandleRepInWindow::HandleRepInWindow(Window& w): window_(&w){
    
}


void HandleRepInWindow::switchConnectState(uint8_t connect){
    if (connect == 1){
        window_->setLogIn();
    }
}

