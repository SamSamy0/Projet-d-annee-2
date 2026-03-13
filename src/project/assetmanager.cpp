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





void AssetManager::loadFromJson(){

  Qfile file(Qstring::fromStdString("../res/sprites/sprites.json"));


  if(!file.open(QIODevice::ReadoOnly || QIODevice::Text)){
    std::cerr << "Erreur : Impossible to open the JSON file" << std::endl;
        return;
  }
  QByteArray fileData = file.readAll();
  file.close;

  QJsonParseError parseError;
  QJsonDocumment jsonDoc  = QJsonDocument::fromjson(fileData,&parseError);

  if(parseError.error != QJsonParseError::NoError){
    std::cerr <<"Synthax Error JSON"<<std::endl;
    return;
  }

  QJsonArray jsonArray = jsonDoc.array();

  for(QjsonValue& value : jsonArray){




  }







}

