#pragma once
#include "../client.hpp"
#include <SFML/Network.hpp>
#include <iostream>

class Worker;

class IMessage {
public:
  virtual void process(Worker &worker) = 0;
  virtual ~IMessage() = default;
};

struct LoginMessage : public IMessage {
  std::shared_ptr<Client> client_;
  std::string pseudo_;
  std::string password_;

  LoginMessage(sf::Packet &dataPacket, std::shared_ptr<Client> client);
  void process(Worker &worker) override;
};

struct RegisterMessage : IMessage {
  std::shared_ptr<Client> client_;
  std::string pseudo_;
  std::string password_;

  RegisterMessage(sf::Packet &dataPacket, std::shared_ptr<Client> client);
  void process(Worker &worker) override;
};

struct CreateProjectMessage : IMessage {
  uint userId_;
  std::string nomProjet_;
  sf::Vector2u size_;
  uint scale_;

  CreateProjectMessage(sf::Packet &dataPacket, uint userId);
  void process(Worker &worker) override;
};

struct RenameProjectMessage : IMessage {
  uint userID_;
  uint projectId_;
  std::string newName_;

  RenameProjectMessage(sf::Packet &dataPacket, uint userId);
  void process(Worker &worker) override;
};

struct DuplicateProjectMessage : IMessage {
  uint userId_;
  uint projectId_;
  std::string newName;

  DuplicateProjectMessage(sf::Packet &dataPacket, uint userId);
  void process(Worker &worker) override;
};

struct GetProjectsListMessage : IMessage {
  uint userId_;

  GetProjectsListMessage(sf::Packet &dataPacket, uint userId);
  void process(Worker &worker) override;
};

struct DeleteProjectMessage : IMessage {
  uint userId_;
  uint projectId_;

  DeleteProjectMessage(sf::Packet &dataPacket, uint userId);
  void process(Worker &worker) override;
};

struct GetProjectDataMessage : IMessage {
  uint userId_;
  uint projectId_;

  GetProjectDataMessage(sf::Packet &dataPacket, uint userId);
  void process(Worker &worker) override;
};

struct ModifProjetMessage : IMessage {
  uint userId_;
  uint projectId_;
  uint calqueId_;
  std::vector<uint> getUserLists(Worker &worker);
};

struct CreateLayerMessage : ModifProjetMessage{
  uint8_t type_;
  CreateLayerMessage(sf::Packet &dataPacket, uint userId);
  void process(Worker &worker) override;
};


struct DeleteLayerMessage : ModifProjetMessage{
  DeleteLayerMessage(sf::Packet &dataPacket, uint userId);
  void process(Worker &worker) override;
};

struct PutPixelsMessage : ModifProjetMessage {
  sf::Vector2u pos_;
  uint8_t red_;
  uint8_t green_;
  uint8_t blue_;
  uint8_t opa_;
};

struct PutPixelsCircleMessage : PutPixelsMessage {
  float taille_;

  PutPixelsCircleMessage(sf::Packet &dataPacket, uint userId);
  void process(Worker &worker) override;
};

struct PutPixelsSquareMessage : PutPixelsMessage {
  float taille_;

  PutPixelsSquareMessage(sf::Packet &dataPacket, uint userId);
  void process(Worker &worker) override;
};

struct PutPixelsDiamondMessage : PutPixelsMessage {
  float hauteur_;
  float largeur_;

  PutPixelsDiamondMessage(sf::Packet &dataPacket, uint userId);
  void process(Worker &worker) override;
};

struct ErasePixelsMessage : ModifProjetMessage {
  sf::Vector2u pos_;
};

struct ErasePixelsCircleMessage : ErasePixelsMessage {
  float taille_;

  ErasePixelsCircleMessage(sf::Packet &dataPacket, uint userId);
  void process(Worker &worker) override;
};

struct ErasePixelsSquareMessage : ErasePixelsMessage {
  float taille_;

  ErasePixelsSquareMessage(sf::Packet &dataPacket, uint userId);
  void process(Worker &worker) override;
};

struct ErasePixelsDiamondMessage : ErasePixelsMessage {
  float hauteur_;
  float largeur_;

  ErasePixelsDiamondMessage(sf::Packet &dataPacket, uint userId);
  void process(Worker &worker) override;
};

struct PutSpriteMessage : ModifProjetMessage{
  sf::Vector2u pos_;
  float taille_;
  std::string asset_id;
  PutSpriteMessage(sf::Packet &dataPacket, uint userId);
  void process(Worker &worker) override;
};

struct EraseSpriteMessage : ModifProjetMessage{
  sf::Vector2u pos_;
};

struct EraseSpriteSquareMessage : EraseSpriteMessage{
  float taille_;
  EraseSpriteSquareMessage(sf::Packet &dataPacket, uint userId);
  void process(Worker &worker) override;
};

struct EraseSpriteCircleMessage : EraseSpriteMessage{
  float taille_;

  EraseSpriteCircleMessage(sf::Packet &dataPacket, uint userId);
  void process(Worker &worker) override;
};

struct EraseSpriteDiamondMessage : EraseSpriteMessage{
  float hauteur_;
  float largeur_;

  EraseSpriteDiamondMessage(sf::Packet &dataPacket, uint userId);
  void process(Worker &worker) override;
};

struct MoveLayerMessage : ModifProjetMessage {
  int deltaX_;
  int deltaY_;

  MoveLayerMessage(sf::Packet &dataPacket, uint userId);
  void process(Worker &worker) override;
};

struct GenerateTokenMessage : IMessage {
  uint userId_;
  uint8_t role;
  uint projectId_;

  GenerateTokenMessage(sf::Packet &dataPacket, uint userId);
  void process(Worker &worker) override;
};

struct CheckTokenMessage : IMessage {
  uint userId_;
  std::string token;
  CheckTokenMessage(sf::Packet &dataPacket, uint userId);
  void process(Worker &worker) override;
};

std::unique_ptr<IMessage> MessageFactory(sf::Packet &data_packet,
                                         std::shared_ptr<Client> client);
