#include "handler.hpp"
#include "clientnetwork.hpp"


ClientEventHandler::ClientEventHandler(ClientNetworkManager& client_manager): manager_(&client_manager){}

void ClientEventHandler::trateEventQueu(){
    while ((*manager_).hasEvent()){
        ServerEvent event = (*manager_).popEvent();
        process(event);
    }
}

void ClientEventHandler::process(ServerEvent& event){
    switch(event.message_type_){
        case MsgProtocole::AUTH_RESULT:
            handleAuth(event);
            //mettre le bool IsLoggedIn a true
            break;
        case MsgProtocole::LOB_PROJECT_LIST_REP:
            handleProjectData(event);
            break;
    }        
}




HandleRepInWindow::HandleRepInWindow(Window& w): window_(&w){};

void HandleRepInWindow::switchConnectState(bool connect){
    
}

