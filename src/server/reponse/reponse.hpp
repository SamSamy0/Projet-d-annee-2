#pragma once
#include <vector>
#include <utility>
#include <SFML/Network.hpp>
#include "../datamanager/projectentry.hpp"
#include "../client.hpp"

class ServerNetworkManager;

struct Reponse {
    sf::Packet dataPacket_;
    virtual ~Reponse() = default;

    virtual void envoyer(ServerNetworkManager& servManager) = 0; 
};


struct ReponseSolo : Reponse {
    long long userId_;

    protected:
    ReponseSolo(long long id);
    virtual void envoyer(ServerNetworkManager& servManager) override;
};

struct ReponseAuth : ReponseSolo {
    std::shared_ptr<Client> client_;

    ReponseAuth(std::shared_ptr<Client> client, long long userId);
    virtual void envoyer(ServerNetworkManager& servManager) override;
};

struct ReponseRenameProject: ReponseSolo{

    ReponseRenameProject(long long userID_, int projectId_, std::string newName, bool success);
    // virtual void envoyer(ServerNetworkManager& servManager) override;
};

struct ReponseDuplicateProject: ReponseSolo{
    ReponseDuplicateProject(long long userId_, int projectId_, std::string newName, bool success);
};

struct ReponseProjectData : ReponseSolo {
    ReponseProjectData(long long userId);
};

struct ReponseUsersProjects : ReponseSolo {
    ReponseUsersProjects(long long userId, std::vector<ProjectEntry>& projects);
};



struct ReponseGroupe : Reponse {
    std::vector<long long> usersId_;

    protected:
    ReponseGroupe(std::vector<long long> usersId);
    virtual void envoyer(ServerNetworkManager& servManager) override;
};
