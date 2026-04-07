#include "clientnetwork.hpp"
#include <iostream>

std::deque<ServerEvent> &ClientNetworkManager::getQueuEvent() {
  return reponse_;
}

bool ClientNetworkManager::connect() {
  socket_.setBlocking(true);

  if (socket_.connect({127, 0, 0, 1}, 5001) == sf::Socket::Status::Done) {
    socket_.setBlocking(false);
    return true;
  } else
    return false;
}

void ClientNetworkManager::getEvent() {
  ServerEvent rep;
  auto packet = std::make_unique<sf::Packet>();

  if (socket_.receive(*packet) == sf::Socket::Status::Done) {
    uint8_t type_mess;
    *packet >> type_mess;

    rep.message_type_ = static_cast<MsgProtocole>(type_mess);
    rep.data_packet_ = std::move(packet);

    reponse_.push_back(std::move(rep));
  }
}

bool ClientNetworkManager::hasEvent() { return !reponse_.empty(); }

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

  if (socket_.send(packet) != sf::Socket::Status::Done)
    std::cerr << "ERROR : ClientNetWorkManager => " << to_string(msg)
              << std::endl;
}

void ClientNetworkManager::askRegister(std::string pseudo,
                                       std::string password) {
  sf::Packet packet;
  MsgProtocole msg = MsgProtocole::AUTH_REGISTER_REQ;

  packet << static_cast<uint8_t>(msg);
  packet << pseudo << password;

  if (socket_.send(packet) != sf::Socket::Status::Done)
    std::cerr << "ERROR : ClientNetWorkManager => " << to_string(msg)
              << std::endl;
}




void ClientNetworkManager::createProject(std::string nomProjet,
                                         sf::Vector2u size, uint scale) {
  sf::Packet packet;
  MsgProtocole msg = MsgProtocole::LOB_CREATE_PROJECT_REQ;

  packet << static_cast<uint8_t>(msg);
  packet << nomProjet << size.x << size.y << scale;

  if (socket_.send(packet) != sf::Socket::Status::Done)
    std::cerr << "ERROR : ClientNetWorkManager => " << to_string(msg)
              << std::endl;
}

void ClientNetworkManager::renameProject(uint projectId, std::string newName) {
  sf::Packet packet;
  MsgProtocole msg = MsgProtocole::LOB_RENAME_PROJECT_REQ;

  packet << static_cast<uint8_t>(msg);
  packet << projectId << newName;

  if (socket_.send(packet) != sf::Socket::Status::Done)
    std::cerr << "ERROR : ClientNetWorkManager => " << to_string(msg)
              << std::endl;
}

void ClientNetworkManager::dupProj(uint projectId, std::string newName) {
  sf::Packet packet;
  MsgProtocole msg = MsgProtocole::LOB_DUPLICATE_PROJECT_REQ;

  packet << static_cast<uint8_t>(msg);
  packet << projectId << newName;
  if (socket_.send(packet) != sf::Socket::Status::Done)
    std::cerr << "ERROR : ClientNetWorkManager => " << to_string(msg)
              << std::endl;
}

void ClientNetworkManager::getProjectList() {
  sf::Packet packet;
  MsgProtocole msg = MsgProtocole::LOB_PROJECT_LIST_REQ;

  packet << static_cast<uint8_t>(msg);
  if (socket_.send(packet) != sf::Socket::Status::Done)
    std::cerr << "ERROR : ClientNetWorkManager => " << to_string(msg)
              << std::endl;
}

void ClientNetworkManager::getProjectData(uint project_id) {
  sf::Packet packet;
  MsgProtocole msg = MsgProtocole::LOB_GET_PROJECT_DATA_REQ;

  packet << static_cast<uint8_t>(msg);
  packet << static_cast<uint>(project_id);
  packet << project_id;
  if (socket_.send(packet) != sf::Socket::Status::Done)
    std::cerr << "ERROR : ClientNetWorkManager => " << to_string(msg)
              << std::endl;
}

void ClientNetworkManager::delProject(uint project_id) {
  sf::Packet packet;
  MsgProtocole msg = MsgProtocole::LOB_DEL_PROJECT_REQ;

  packet << static_cast<uint8_t>(msg);
  packet << project_id;
  if (socket_.send(packet) != sf::Socket::Status::Done)
    std::cerr << "ERROR : ClientNetWorkManager => " << to_string(msg)
              << std::endl;
}

void ClientNetworkManager::createProjectCode(uint8_t role, uint project_id) {
  sf::Packet packet;
  MsgProtocole msg = MsgProtocole::LOB_SHARE_PROJECT_REQ;

  packet << static_cast<uint8_t>(msg);
  packet << static_cast<uint8_t>(role);
  packet << static_cast<uint32_t>(project_id);
  if (socket_.send(packet) != sf::Socket::Status::Done)
    std::cerr << "ERROR : ClientNetWorkManager => " << to_string(msg)
              << std::endl;
}

void ClientNetworkManager::joinProject(std::string project_code) {
  sf::Packet packet;
  MsgProtocole msg = MsgProtocole::LOB_JOIN_PROJECT_REQ;

  packet << static_cast<uint8_t>(msg);
  packet << project_code;
  if (socket_.send(packet) != sf::Socket::Status::Done)
    std::cerr << "ERROR : ClientNetWorkManager => " << to_string(msg)
              << std::endl;
}

void ClientNetworkManager::createLayer(uint proj_id,LayerType type){
  sf::Packet packet;
  MsgProtocole msg = MsgProtocole::MAP_CREATE_LAYER_REQ;

  packet << static_cast<uint8_t>(msg);
  uint8_t type_int = static_cast<uint8_t>(type);
  packet << proj_id << type_int;

  if (socket_.send(packet) != sf::Socket::Status::Done)
    std::cerr << "ERROR : ClientNetWorkManager => " << to_string(msg)
              << std::endl;
}



void ClientNetworkManager::deleteLayer(uint proj_id, uint layer_id){
  sf::Packet packet;
  MsgProtocole msg = MsgProtocole::MAP_REMOVE_LAYER_REQ;

  packet << static_cast<uint8_t>(msg);
  packet << proj_id << layer_id;

  if (socket_.send(packet) != sf::Socket::Status::Done)
    std::cerr << "ERROR : ClientNetWorkManager => " << to_string(msg)
              << std::endl;
}

void ClientNetworkManager::renameLayer(uint proj_id, uint layer_id, std::string name){
  sf::Packet packet;
  MsgProtocole msg = MsgProtocole::MAP_RENAME_LAYER_REQ;

  packet << static_cast<uint8_t>(msg);
  packet << proj_id << layer_id << name;

  if (socket_.send(packet) != sf::Socket::Status::Done)
    std::cerr << "ERROR : ClientNetWorkManager => " << to_string(msg)
              << std::endl;
}



void ClientNetworkManager::layerUp(uint proj_id, uint layer_id){
  sf::Packet packet;
  MsgProtocole msg = MsgProtocole::MAP_ORGANIZE_LAYER_UP_REQ;

  packet << static_cast<uint8_t>(msg);
  packet << proj_id << layer_id;

  if (socket_.send(packet) != sf::Socket::Status::Done)
    std::cerr << "ERROR : ClientNetWorkManager => " << to_string(msg)
              << std::endl;
}

void ClientNetworkManager::layerDown(uint proj_id, uint layer_id){
  sf::Packet packet;
  MsgProtocole msg = MsgProtocole::MAP_ORGANIZE_LAYER_DOWN_REQ;

  packet << static_cast<uint8_t>(msg);
  packet << proj_id << layer_id;

  if (socket_.send(packet) != sf::Socket::Status::Done)
    std::cerr << "ERROR : ClientNetWorkManager => " << to_string(msg)
              << std::endl;
}

void ClientNetworkManager::drawSquare(uint proj_id, uint layer_id, int pos_x,
                                      int pos_y, float size, uint8_t r,
                                      uint8_t g, uint8_t b, uint8_t a) {
  sf::Packet packet;
  MsgProtocole msg = MsgProtocole::MAP_PUT_PIXELS_SQUARE_REQ;

  packet << static_cast<uint8_t>(msg);
  packet << proj_id << layer_id << pos_x << pos_y << size << r << g << b << a;

  if (socket_.send(packet) != sf::Socket::Status::Done)
    std::cerr << "ERROR : ClientNetWorkManager => " << to_string(msg)
              << std::endl;
}



void ClientNetworkManager::drawCircle(uint proj_id, uint layer_id, int pos_x,
                                      int pos_y, float size, uint8_t r,
                                      uint8_t g, uint8_t b, uint8_t a) {
  sf::Packet packet;
  MsgProtocole msg = MsgProtocole::MAP_PUT_PIXELS_CIRCLE_REQ;

  packet << static_cast<uint8_t>(msg);
  packet << proj_id << layer_id << pos_x << pos_y << size << r << g << b << a;

  if (socket_.send(packet) != sf::Socket::Status::Done)
    std::cerr << "ERROR : ClientNetWorkManager => " << to_string(msg)
              << std::endl;
}

void ClientNetworkManager::drawDiamond(uint proj_id, uint layer_id, int pos_x,
                                       int pos_y, float size_x, float size_y,
                                       uint8_t r, uint8_t g, uint8_t b,
                                       uint8_t a) {
  sf::Packet packet;
  MsgProtocole msg = MsgProtocole::MAP_PUT_PIXELS_DIAM_REQ;

  packet << static_cast<uint8_t>(msg);
  packet << proj_id << layer_id << pos_x << pos_y << size_y << size_x
        << r << g << b << a;

  if (socket_.send(packet) != sf::Socket::Status::Done)
    std::cerr << "ERROR : ClientNetWorkManager => " << to_string(msg)
              << std::endl;
}

void ClientNetworkManager::eraseSquare(uint proj_id, uint layer_id, int pos_x,
                                       int pos_y, float size) {
  sf::Packet packet;
  MsgProtocole msg = MsgProtocole::MAP_ERASE_PIXELS_SQUARE_REQ;

  packet << static_cast<uint8_t>(msg);
  packet << proj_id << layer_id << pos_x << pos_y << size;

  if (socket_.send(packet) != sf::Socket::Status::Done)
    std::cerr << "ERROR : ClientNetWorkManager => " << to_string(msg)
              << std::endl;
}

void ClientNetworkManager::eraseCircle(uint proj_id, uint layer_id, int pos_x,
                                       int pos_y, float size) {
  sf::Packet packet;
  MsgProtocole msg = MsgProtocole::MAP_ERASE_PIXELS_CIRCLE_REQ;

  packet << static_cast<uint8_t>(msg);
  packet << proj_id << layer_id << pos_x << pos_y << size;

  if (socket_.send(packet) != sf::Socket::Status::Done)
    std::cerr << "ERROR : ClientNetWorkManager => " << to_string(msg)
              << std::endl;
}

void ClientNetworkManager::eraseDiamond(uint proj_id, uint layer_id, int pos_x,
                                        int pos_y, float size_x, float size_y) {
  sf::Packet packet;
  MsgProtocole msg = MsgProtocole::MAP_ERASE_PIXELS_DIAM_REQ;

  packet << static_cast<uint8_t>(msg);
  packet << proj_id << layer_id << pos_x << pos_y << size_y << size_x;

  if (socket_.send(packet) != sf::Socket::Status::Done)
    std::cerr << "ERROR : ClientNetWorkManager => " << to_string(msg)
              << std::endl;
}

void ClientNetworkManager::shiftLayer(uint proj_id, uint layer_id, int delta_x,
                                      int delta_y) {
  sf::Packet packet;
  MsgProtocole msg = MsgProtocole::MAP_MOV_LAYER_REQ;

  packet << static_cast<uint8_t>(msg);
  packet << proj_id << layer_id << delta_x << delta_y;

  if (socket_.send(packet) != sf::Socket::Status::Done)
    std::cerr << "ERROR : ClientNetWorkManager => " << to_string(msg)
              << std::endl;
}


void ClientNetworkManager::drawSprite(uint proj_id, uint layer_id, std::string asset_id, int pos_x, int pos_y, float size_x){

  sf::Packet packet;
  MsgProtocole msg = MsgProtocole::MAP_PUT_SPRITE_REQ;

  packet << static_cast<uint8_t>(msg);
  packet << proj_id << layer_id << asset_id << pos_x<<pos_y<<size_x;


  if (socket_.send(packet) != sf::Socket::Status::Done)
    std::cerr << "ERROR : ClientNetWorkManager => " << to_string(msg)
              << std::endl;
}

void ClientNetworkManager::eraseSprite(uint proj_id, uint layer_id, uint sprite_id) {
  sf::Packet packet;
  MsgProtocole msg = MsgProtocole::MAP_ERASE_SPRITE_REQ;

  packet << static_cast<uint8_t>(msg);
  packet << proj_id << layer_id << sprite_id ;


  if (socket_.send(packet) != sf::Socket::Status::Done)
    std::cerr << "ERROR : ClientNetWorkManager => " << to_string(msg)
              << std::endl;
}

