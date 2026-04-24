#pragma once
#include "../client.hpp"
#include "../datamanager/memberentry.hpp"
#include "../datamanager/projectentry.hpp"
#include "../message/message.hpp"
#include "../SpriteLayer.hpp"

class ServerNetworkManager;

struct Reponse {
  sf::Packet dataPacket_;
  virtual ~Reponse() = default;

  virtual void envoyer(ServerNetworkManager &servManager) = 0;
};

struct ReponseSolo : Reponse {
  uint userId_;

protected:
  ReponseSolo(uint id);
  virtual void envoyer(ServerNetworkManager &servManager) override;
};

struct ReponseAuth : ReponseSolo {
  std::shared_ptr<Client> client_;

  ReponseAuth(std::shared_ptr<Client> client, uint userId);
  virtual void envoyer(ServerNetworkManager &servManager) override;
};

struct ReponseDeconnection : ReponseSolo {

    ReponseDeconnection(uint userId);
    virtual void envoyer(ServerNetworkManager& servManager) override;
};

struct ReponseRenameProject: ReponseSolo{

    ReponseRenameProject(uint userID_, uint projectId_, std::string newName, bool success);
    // virtual void envoyer(ServerNetworkManager& servManager) override;
};

struct ReponseDuplicateProject : ReponseSolo {
  ReponseDuplicateProject(uint userId_, uint projectId_, std::string newName);
};
struct ReponseGenerateToken : ReponseSolo {
  ReponseGenerateToken(uint userId_, std::string token);
};

struct ReponseJoinProject : ReponseSolo {
  ReponseJoinProject(uint userId_, bool success);
};
struct ReponseGetMember : ReponseSolo {
  ReponseGetMember(uint userId_, std::vector<MemberEntry> memberList);
};

struct ReponseProjectData : ReponseSolo {
    ReponseProjectData(uint userId, const QJsonObject& jsonDoc, 
                   const std::vector<uint>& layerOrder, 
                   const std::unordered_map<uint, QImage>& imageMap, 
                   const std::unordered_map<uint, SpriteLayer>& spriteMap, const QJsonArray& chat);
    static QByteArray imageToBytes(const QImage& image);
};


struct ReponseUsersProjects : ReponseSolo {
  ReponseUsersProjects(uint userId, std::vector<ProjectEntry> &projects);
};

struct ReponseCreateProject : ReponseSolo {
  ReponseCreateProject(uint userId, uint projectId_);
};

struct ReponseGroupe : Reponse {
  std::vector<uint> usersId_;

protected:
  ReponseGroupe(std::vector<uint> usersId);
  virtual void envoyer(ServerNetworkManager &servManager) override;
};


struct ReponseCreateLayer : ReponseGroupe{
   ReponseCreateLayer(std::vector<uint> usersId, CreateLayerMessage& mess);
};


struct ReponseDeleteLayer : ReponseGroupe{
   ReponseDeleteLayer(std::vector<uint> usersId, DeleteLayerMessage& mess);
};


struct ReponseRenameLayer : ReponseGroupe{
   ReponseRenameLayer(std::vector<uint> usersId, RenameLayerMessage& mess);
};


struct ReponseOrganizeLayerUp : ReponseGroupe{
    ReponseOrganizeLayerUp(std::vector<uint> usersId, OrganizeLayerUpMessage& mess);
};

struct ReponseOrganizeLayerDown : ReponseGroupe{
    ReponseOrganizeLayerDown(std::vector<uint> usersId, OrganizeLayerDownMessage& mess);
};

struct ReponseChangeRole : ReponseGroupe {
  ReponseChangeRole(std::vector<uint> usersId_, uint target, uint projectId_,
                    int8_t role, bool success);
};

struct ReponsePutPixelsCircle : ReponseGroupe {
  ReponsePutPixelsCircle(std::vector<uint> usersId,
                         PutPixelsCircleMessage &mess);
};

struct ReponsePutPixelsSquare : ReponseGroupe {
  ReponsePutPixelsSquare(std::vector<uint> usersId,
                         PutPixelsSquareMessage &mess);
};

struct ReponsePutPixelsDiamond : ReponseGroupe {
  ReponsePutPixelsDiamond(std::vector<uint> usersId,
                          PutPixelsDiamondMessage &mess);
};

struct ReponseErasePixelsCircle : ReponseGroupe {
  ReponseErasePixelsCircle(std::vector<uint> usersId,
                           ErasePixelsCircleMessage &mess);
};

struct ReponseErasePixelsSquare : ReponseGroupe {
  ReponseErasePixelsSquare(std::vector<uint> usersId,
                           ErasePixelsSquareMessage &mess);
};

struct ReponseErasePixelsDiamond : ReponseGroupe {
  ReponseErasePixelsDiamond(std::vector<uint> usersId,
                            ErasePixelsDiamondMessage &mess);
};

struct ReponsePutSprite : ReponseGroupe {
  ReponsePutSprite(std::vector<uint> usersId, PutSpriteMessage &mess);
};

struct ReponseEraseSprite : ReponseGroupe {

    ReponseEraseSprite(std::vector<uint> usersId, EraseSpriteMessage& mess);
};

struct ReponseMoveLayer : ReponseGroupe {
  ReponseMoveLayer(std::vector<uint> usersId, MoveLayerMessage &mess);
};

struct ReponseChat : ReponseGroupe {
  ReponseChat(std::vector<uint> usersId,ChatMessage& mess);
};
