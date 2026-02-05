#include <SFML/Network.hpp>


class ClientNetworkManager{
    sf::TcpSocket socket;
    
    public:
        
        void connect(){
            socket.connect({127,0,0,1},5000);
        }


};

int main(){
    
    return 1;
};