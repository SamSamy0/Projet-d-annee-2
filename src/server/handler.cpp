#include "handler.hpp"


void ClientEventHandler::process(ServerRep& event){
    switch(event.message_type){
        case MsgProtocole::AUTH_RESULT:
            handleAuth(event);
            //mettre le bool IsLoggedIn a true
            break;
        case MsgProtocole::LOB_PROJECT_DATA_REP:
            handleProjectData(event);
            break;
    }
};

