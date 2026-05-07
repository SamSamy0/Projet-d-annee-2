#include "spriteManager.hpp"
#include <iostream>
#include <QJsonObject>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QFileInfo>
#include <QDebug>
#include <QJsonArray>
#include <QString>
#include "datamanager/projectsmanager.hpp"

SpriteManager::SpriteManager(std::map<uint, sf::Texture> importedSprite) : importedSprite_(importedSprite) {
  loadFromJson();
  for (const auto& [id, texture] : importedSprite_) {
    assetsId_.insert("import_" + std::to_string(id));
    nextId_ += 1;
  }
}

SpriteManager::SpriteManager() {
  loadFromJson();
}

void SpriteManager::loadFromJson(){
  std::string spritesPath = "../res/sprites/sprites.json";

  QFile file(QString::fromStdString(spritesPath));


  if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
    std::cerr << "Erreur : Impossible to open the JSON file" << std::endl;
        return;
  }
  QByteArray fileData = file.readAll();
  file.close();

  QJsonParseError parseError;
  QJsonDocument jsonDoc  = QJsonDocument::fromJson(fileData,&parseError);

  if(parseError.error != QJsonParseError::NoError){
    std::cerr <<"Synthax Error JSON"<<std::endl;
    return;
  }

  QJsonArray jsonArray = jsonDoc.object()["sprites"].toArray();

  for(QJsonValueRef value : jsonArray){
      QJsonObject item = value.toObject();

      assetsId_.insert(item["id"].toString().toStdString());
  }
}

uint SpriteManager::addSprite(const sf::Texture& texture) {
  uint id = nextId_;
  nextId_++;
  std::string assetId = "import_" + std::to_string(id);
  assetsId_.insert(assetId);
  importedSprite_[id] = texture; // Stocke la texture dans la map
  return id;
}

std::map<uint, sf::Texture>& SpriteManager::getImportedSprites() {
  return importedSprite_;
}

bool SpriteManager::verifyAssetId(const std::string& assetId) {
    return assetsId_.find(assetId) != assetsId_.end();
}