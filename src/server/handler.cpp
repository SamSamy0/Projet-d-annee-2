#include <SFML/Network.hpp>
#include <iostream>
#include <deque>
#include <memory>
#include "protocol.hpp"
#include "clientnetwork.hpp"


void ClientEventHandler::process(ServerRep& event){
    switch(event.message_type){
        case AUTH_RESULT:
            handleAuth(event);
            break;
    }
};