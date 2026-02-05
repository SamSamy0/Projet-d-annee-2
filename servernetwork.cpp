#include <SFML/Network.hpp>
#include <vector>
#include <iostream>
#include <array>


struct Client{
    sf::IpAddress address_ip;
    unsigned short port;
};

class ServerNetworkManager{
    sf::TcpListener listener;
    std::vector<sf::TcpSocket> liste_client;
    std::vector<std::array<char, 10>> messages;
    public:
        void start(){
            listener.listen(5000);
        };

        void accept(){
            sf::TcpSocket client;
            listener.accept(client);
            liste_client.push_back(client);
        };

        void getmessages(){
            for (sf::TcpSocket& client : liste_client){
                std::array<char,10> data;
                std::size_t received;
                client.receive(&data,10,received);
                messages.push_back(data);
            };
        };

        void tratemessages(){
            for (std::array<char,10>&  message : messages){
                std::cout.write(message.data(),10);
                std::cout << '\n';
            };
        };

        void reponse(){
            for (sf::TcpSocket& client : liste_client){
                client.send("messages recu",13);
            };
        };

    
};

int main(){   // juste pour tester
    ServerNetworkManager server;
    server.start();
    for (int i =0; i< 5; i++){
        server.accept();
        server.getmessages();
        server.tratemessages();
        server.reponse();
    };
    
    return 0;
}