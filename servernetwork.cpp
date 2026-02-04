#include <SFML/Network.hpp>


class ServerNetworkManager{
    sf::TcpListener listener;
    public:
        ServerNetworkManager(){
            listener.listen(5000);
        };
    
};

