#pragma once
#include <vector>
#include <utility>
#include <SFML/Network.hpp>
#include "../datamanager/projectentry.hpp"
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
    std::shared_ptr<Client> client_;

    ReponseAuth(std::shared_ptr<Client> client, long long userId);
    void envoyer() override;
};

struct ReponseProjectData : ReponseSolo {
    ReponseProjectData(long long userId);
    void envoyer() override;
};

struct ReponseUsersProjects : ReponseSolo {
    ReponseUsersProjects(long long userId, std::vector<ProjectEntry>& projects);
    void envoyer() override;
};



struct ReponseGroupe : Reponse {
    std::vector<long long> usersId_;

    protected:
    ReponseGroupe(std::vector<long long> usersId);
};