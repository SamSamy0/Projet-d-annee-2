#include "handler.hpp"

// ClientEventHandler::ClientEventHandler(ClientNetworkManager& client_manager):
// manager_(&client_manager){}
//
// void ClientEventHandler::trateEventQueu(){
//     while ((*manager_).hasEvent()){
//         ServerEvent event = (*manager_).popEvent();
//         process(event);
//     }
// }
//
// void ClientEventHandler::process(ServerEvent& event){
//     switch(event.message_type){
//         case MsgProtocole::AUTH_RESULT:
//             handleAuth(event);
//             //mettre le bool IsLoggedIn a true
//             break;
//         case MsgProtocole::LOB_PROJECT_DATA_REP:
//             handleProjectData(event);
//             break;
//         case MsgProtocole::LOB_GET_MY_PROJECTS_DATA_REP:
//             handleMyProjects(event);
//             break;
//         }
// }
//
// void handleMyProjects(ServerEvent& event) {
//
// }
//
