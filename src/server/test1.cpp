#include <SFML/Network.hpp>
#include "protocol.hpp"
#include <iostream>

int main(){
    sf::TcpSocket sock;
    sock.connect({127,0,0,1},5000);
    sf::Packet pack;
    std::string pseudo = "tomas";
    std::string pass = "Tom123";
    pack << static_cast<uint8_t>(MsgProtocole::AUTH_LOGIN_REQ) << pseudo << pass;

    sock.send(pack);

    sf::Packet recu;
    if (sock.receive(recu) == sf::Socket::Status::Done){
        std::cout << "reussi!!!" <<std::endl;
    }

    uint8_t msg;
    uint8_t reussi;
    recu >> msg >> reussi;
    std::cout << static_cast<int>(reussi) <<std::endl;  

    std::cout << "reussi!!!" <<std::endl;


    return 0;
}