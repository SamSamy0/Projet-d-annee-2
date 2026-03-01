#pragma once
#include <vector>
#include <utility>
#include <SFML/Network.hpp>
#include "projectentry.hpp"
#include "client.hpp"

struct Reponse {
    sf::Packet dataPacket_;
    virtual ~Reponse() = default;

    virtual void envoyer() = 0; 
};




struct ReponseSolo : Reponse {
    long long userId_;

    protected:
    ReponseSolo(long long id);
};

struct ReponseAuth : ReponseSolo {
    std::unique_ptr<sf::TcpSocket> userSocket_;

    ReponseAuth(std::unique_ptr<sf::TcpSocket> socket, long long userId);
    void envoyer() override;
};

struct ReponseProjectData : ReponseSolo {
    ReponseProjectData(long long userId);
    void envoyer() override;
};




struct ReponseGroupe : Reponse {
    std::vector<long long> usersId_;

    protected:
    ReponseGroupe(std::vector<long long> usersId);
};

struct ReponseUsersProjects : ReponseGroupe {
    ReponseUsersProjects(std::vector<long long> usersId, std::vector<ProjectEntry> projects);
    void envoyer() override;
};