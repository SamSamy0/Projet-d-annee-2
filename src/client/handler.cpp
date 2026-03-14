#include "handler.hpp"
#include "clientnetwork.hpp"
#include <iostream>


ClientHandler::ClientHandler(ClientNetworkManager& client_manager,ReceiverInWindow& w)
: manager_(client_manager), handleWindow_(w) {}


void ClientHandler::processEventQueu(){
    while ((manager_).hasEvent()){
        ServerEvent event = (manager_).popEvent();
        process(event);
    }
}


void ClientHandler::process(ServerEvent& event){
    switch(event.message_type_){
        
        case MsgProtocole::AUTH_RESULT:
            uint8_t accept;
            *(event.data_packet_) >> accept;
            handleWindow_.switchConnectState(accept);
            break;
        
        case MsgProtocole::LOB_PROJECT_LIST_REP:
            uint32_t size;
            *(event.data_packet_) >> size;

            for (int i = 0; i< static_cast<int>(size); ++i){

                ProjectData projet;
                uint32_t id;
                std::string name;
                int8_t role;

                *(event.data_packet_) >> id >> name >> role; 
                projet.projectId = id;
                projet.projectName = name;
                projet.role = role;

                handleWindow_.addProjectToList(projet);
            }

            break;
        
        case MsgProtocole::LOB_GET_PROJECT_DATA_REP:
            break;
        case MsgProtocole::LOB_RENAME_PROJECT_REP:{
            uint8_t success;
            uint32_t projectId;
            std::string newName;
            *(event.data_packet_) >> success >> projectId >> newName;
            if (success){
                handleWindow_.updateProjectNameInList(projectId, newName);
            }
            break;
                
        }
        case MsgProtocole::LOB_DUPLICATE_PROJECT_REP:{
            uint8_t success;
            uint32_t projectId;
            std::string newName;
            *(event.data_packet_) >> success >> projectId >> newName;
            if (success){
                ProjectData projet;

                projet.projectId = projectId;
                projet.projectName = newName;
                //Owner
                projet.role = 2;

                handleWindow_.addProjectToList(projet);
                
            }
            break;
                    }
            
    }        
}





