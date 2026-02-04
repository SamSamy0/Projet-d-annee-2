#include <SFML/Network.hpp>


class ClientNetworkManager{
    sf::TcpSocket socket;
    sf::Socket::Status status;
    public:
        ClientNetworkManager(){
            status = socket.connect({127,0,0,1},5000);
        }

};