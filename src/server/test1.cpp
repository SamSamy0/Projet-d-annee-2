#include <SFML/Network.hpp>
#include "protocol.hpp"
#include <iostream>

int main(){
    sf::TcpSocket sock;
    sock.connect({127,0,0,1},5000);
    sf::Packet pac;
    std::string psuedo = "Chinzorrr";
    std::string pass = "motdepasse";
    pac << static_cast<uint8_t>(MsgProtocole::AUTH_LOGIN_REQ) << psuedo << pass;
    
    sock.send(pac);


    sf::Packet pack;
    std::string nom_proj = "Proj_1_omg";
    sf::Vector2u taille;
    taille.x = 1200;
    taille.y = 800;
    uint scale = 5;
    
    pack << static_cast<uint8_t>(MsgProtocole::LOB_CREATE_PROJECT_REQ) << nom_proj << taille.x << taille.y << scale;

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