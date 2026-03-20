#include "assetmanager.hpp"
#include <iostream>
#include <QJsonObject>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QFileInfo>
#include <QDebug>
#include <QJsonArray>
#include <QString>

AssetManager::AssetManager(){loadFromJson();}




void AssetManager::loadFromJson(){
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
    
    Asset newAsset;

    newAsset.id = item["id"].toString().toStdString();
    newAsset.filename = item["filename"].toString().toStdString();
    newAsset.category = item["category"].toString().toStdString();
    newAsset.name = item["name"].toString().toStdString();
    newAsset.size_m_horizontal = static_cast<float>(item["size_meters_horizontal"].toDouble());

    newAsset.texture = std::make_unique<sf::Texture>();
    std::string imagePath = "../res/sprites/" + newAsset.filename;

    if(newAsset.texture->loadFromFile(imagePath)){
      assets_[newAsset.id] = std::move(newAsset);
    }
    else{
      std::cerr << "Error : Can not load the image : " << imagePath<<std::endl; 
    }

  }

}



  Asset* AssetManager::getAsset(const std::string& id){
  return &assets_[id];
}
