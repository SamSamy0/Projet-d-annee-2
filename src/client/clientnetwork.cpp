#include "clientnetwork.hpp"
#include <iostream>

std::deque<ServerEvent> &ClientNetworkManager::getQueuEvent() {
  return reponse_;
}

bool ClientNetworkManager::connect() {
  socket_.setBlocking(true);

  if (socket_.connect({127, 0, 0, 1}, 5000) == sf::Socket::Status::Done) {
    socket_.setBlocking(false);
    return true;
  } else
    return false;
}

void ClientNetworkManager::getEvent() {
  ServerEvent rep;
  auto packet = std::make_unique<sf::Packet>();

  if (socket_.receive(*packet) == sf::Socket::Status::Done) {
    std::cout << "####################  messages recu du serveur  "
                 "########################"
              << std::endl;
    uint8_t type_mess;
    *packet >> type_mess;

    rep.message_type_ = static_cast<MsgProtocole>(type_mess);
    rep.data_packet_ = std::move(packet);

    reponse_.push_back(std::move(rep));
  }
}

bool ClientNetworkManager::hasEvent(){
    return !reponse_.empty();
}


ServerEvent ClientNetworkManager::popEvent() {
    auto msg = std::move(reponse_.front());
    reponse_.pop_front();
    return msg;
}


void ClientNetworkManager::login(std::string pseudo, std::string password) {
  sf::Packet packet;
  MsgProtocole msg = MsgProtocole::AUTH_LOGIN_REQ;

  packet << static_cast<uint8_t>(msg);
  packet << pseudo << password;

  socket_.send(packet);
}

void ClientNetworkManager::askRegister(std::string pseudo,
                                       std::string password) {
  sf::Packet packet;
  MsgProtocole msg = MsgProtocole::AUTH_REGISTER_REQ;

  packet << static_cast<uint8_t>(msg);
  packet << pseudo << password;

  socket_.send(packet);
}

void ClientNetworkManager::createProject(std::string nomProjet,
                                         sf::Vector2u size, uint scale) {
  sf::Packet packet;
  MsgProtocole msg = MsgProtocole::LOB_CREATE_PROJECT_REQ;

  packet << static_cast<uint8_t>(msg);
  packet << nomProjet << size.x << size.y << scale;

  socket_.send(packet);
}
void ClientNetworkManager::renameProject(int projectId, std::string newName) {
  sf::Packet packet;
  MsgProtocole msg = MsgProtocole::LOB_RENAME_PROJECT_REQ;

  packet << static_cast<uint8_t>(msg);
  packet << projectId << newName;

  socket_.send(packet);
}

void ClientNetworkManager::dupProj(int projectId, std::string newName) {
  sf::Packet packet;
  MsgProtocole msg = MsgProtocole::LOB_DUPLICATE_PROJECT_REQ;

  packet << static_cast<uint8_t>(msg);
  packet << projectId << newName;
  socket_.send(packet);
}

void ClientNetworkManager::getProjectList() {
  sf::Packet packet;
  MsgProtocole msg = MsgProtocole::LOB_PROJECT_LIST_REQ;

  packet << static_cast<uint8_t>(msg);
  socket_.send(packet);
}

void ClientNetworkManager::getProjectData(long long project_id){
    sf::Packet packet;
    MsgProtocole msg = MsgProtocole::LOB_GET_PROJECT_DATA_REQ;

    packet << static_cast<uint8_t>(msg);
    packet << static_cast<int>(project_id);
    socket_.send(packet);
}


void ClientNetworkManager::delProject(long long project_id){
    sf::Packet packet;
    MsgProtocole msg = MsgProtocole::LOB_DEL_PROJECT_REQ;

    packet << static_cast<uint8_t>(msg);
    packet << static_cast<int>(project_id);
    socket_.send(packet);
}

