#pragma once
#include <vector>
#include <utility>
#include <SFML/Network.hpp>
#include "../datamanager/projectentry.hpp"
#include "../client.hpp"
#include <QFile>
#include "../message/message.hpp"

class ServerNetworkManager;

struct Reponse {
    sf::Packet dataPacket_;
    virtual ~Reponse() = default;

    virtual void envoyer(ServerNetworkManager& servManager) = 0; 
};


struct ReponseSolo : Reponse {
    uint userId_;

    protected:
    ReponseSolo(uint id);
    virtual void envoyer(ServerNetworkManager& servManager) override;
};

struct ReponseAuth : ReponseSolo {
    std::shared_ptr<Client> client_;

    ReponseAuth(std::shared_ptr<Client> client, uint userId);
    virtual void envoyer(ServerNetworkManager& servManager) override;
};

struct ReponseRenameProject: ReponseSolo{

    ReponseRenameProject(uint userID_, uint projectId_, std::string newName, bool success);
    // virtual void envoyer(ServerNetworkManager& servManager) override;
};

struct ReponseDuplicateProject: ReponseSolo{
    ReponseDuplicateProject(uint userId_, uint projectId_, std::string newName);
};
struct ReponseGenerateToken: ReponseSolo{
    ReponseGenerateToken(uint userId_, std::string token);
};

struct ReponseJoinProject: ReponseSolo{
    ReponseJoinProject(uint userId_, bool success);
};

struct ReponseProjectData : ReponseSolo {
    ReponseProjectData(uint userId, QByteArray& jsonData);
};

struct ReponseUsersProjects : ReponseSolo {
    ReponseUsersProjects(uint userId, std::vector<ProjectEntry>& projects);
};

struct ReponseCreateProject : ReponseSolo {
    ReponseCreateProject(uint userId, uint projectId_);
};

struct ReponseGroupe : Reponse {
    std::vector<uint> usersId_;

    protected:
    ReponseGroupe(std::vector<uint> usersId);
    virtual void envoyer(ServerNetworkManager& servManager) override;
};


struct ReponseCreateLayer : ReponseGroupe{
   ReponseCreateLayer(std::vector<uint> usersId, CreateLayerMessage& mess);
};

struct ReponsePutPixelsCircle : ReponseGroupe {
    
   ReponsePutPixelsCircle(std::vector<uint> usersId, PutPixelsCircleMessage& mess);
};

struct ReponsePutPixelsSquare : ReponseGroupe {
    
   ReponsePutPixelsSquare(std::vector<uint> usersId, PutPixelsSquareMessage& mess);
};

struct ReponsePutPixelsDiamond : ReponseGroupe {
    
   ReponsePutPixelsDiamond(std::vector<uint> usersId, PutPixelsDiamondMessage& mess);
};

struct ReponseErasePixelsCircle : ReponseGroupe {
    
   ReponseErasePixelsCircle(std::vector<uint> usersId, ErasePixelsCircleMessage& mess);
};

struct ReponseErasePixelsSquare : ReponseGroupe {
    
   ReponseErasePixelsSquare(std::vector<uint> usersId, ErasePixelsSquareMessage& mess);
};

struct ReponseErasePixelsDiamond : ReponseGroupe {
    
   ReponseErasePixelsDiamond(std::vector<uint> usersId, ErasePixelsDiamondMessage& mess);
};

struct ReponsePutSprite : ReponseGroupe{

   ReponsePutSprite(std::vector<uint> usersId, PutSpriteMessage& mess);
};

struct ReponseEraseSpriteSquare : ReponseGroupe {

    ReponseEraseSpriteSquare(std::vector<uint> usersId, EraseSpriteSquareMessage& mess);
};

struct ReponseEraseSpriteCircle : ReponseGroupe {

    ReponseEraseSpriteCircle(std::vector<uint> usersId, EraseSpriteCircleMessage& mess);
};
struct ReponseEraseSpriteDiamond : ReponseGroupe {

    ReponseEraseSpriteDiamond(std::vector<uint> usersId, EraseSpriteDiamondMessage& mess);
};

struct ReponseMoveLayer : ReponseGroupe {
    
   ReponseMoveLayer(std::vector<uint> usersId, MoveLayerMessage& mess);
};
