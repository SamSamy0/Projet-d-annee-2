#include "handler.hpp"
#include "clientnetwork.hpp"
#include <iostream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include <SFML/Network.hpp>
#include <QDebug>

ClientHandler::ClientHandler(ClientNetworkManager& client_manager,ReceiverInWindow& w)
: manager_(client_manager), handleWindow_(w) {}


void ClientHandler::processEventQueu(){
    while ((manager_).hasEvent()){
        ServerEvent event = (manager_).popEvent();
        process(event);
    }
}


void ClientHandler::process(ServerEvent& event){
    switch(event.message_type_){
        
        case MsgProtocole::AUTH_RESULT:{
            uint8_t accept;
            *(event.data_packet_) >> accept;
            handleWindow_.switchConnectState(accept);
            break;
        }


        case MsgProtocole::LOB_PROJECT_LIST_REP:{
            uint32_t size;
            *(event.data_packet_) >> size;

            for (int i = 0; i< static_cast<int>(size); ++i){

                ProjectData projet;
                uint32_t id;
                std::string name;
                int8_t role;

                *(event.data_packet_) >> id >> name >> role; 
                projet.projectId = id;
                projet.projectName = name;
                projet.role = role;

                handleWindow_.addProjectToList(projet);
            }

            break;
        }


        case MsgProtocole::LOB_GET_PROJECT_DATA_REP:{
            std::cout << "DONNEES DU PROJET RECU" << std::endl;

            std::uint32_t jsonSize;
            // On suppose que l'octet d'en-tête (MsgProtocole) a déjà été extrait du flux (stream >>) 
            // juste avant pour déclencher cet événement. On lit donc la taille.
            *(event.data_packet_) >> jsonSize;

            // 1. Calcul du BON offset (1 octet pour le type de message + 4 octets pour la taille)
            size_t offset = sizeof(std::uint8_t) + sizeof(std::uint32_t);
            const char* ptrDonnees = (const char*)(*(event.data_packet_)).getData() + offset;

            // 2. Récupération des données COMPRESSÉES
            QByteArray donneesCompressees(ptrDonnees, jsonSize);

            // 3. DÉCOMPRESSION des données (Étape cruciale qui manquait)
            QByteArray jsonBytes = qUncompress(donneesCompressees);

            // Vérification de sécurité pour s'assurer que la décompression a réussi
            if (jsonBytes.isEmpty()) {
                std::cerr << "Erreur : La décompression a échoué ou les données sont vides." << std::endl;
            } else {
                // 4. Lecture du JSON sur les données en clair
                QJsonDocument doc = QJsonDocument::fromJson(jsonBytes);
                QJsonObject entete = doc.object();

                sf::Vector2u vec{entete["width"].toInt(),entete["height"].toInt()};
                handleWindow_.addProjectData(entete["scale"].toInt(),vec,entete["name"].toString().toStdString(),entete["id"].toInt());

            }
            break;
        }


        case MsgProtocole::LOB_RENAME_PROJECT_REP:{
            uint8_t success;
            uint32_t projectId;
            std::string newName;
            *(event.data_packet_) >> success >> projectId >> newName;
            if (success){
                handleWindow_.updateProjectNameInList(projectId, newName);
            }
            break;
        } 


        case MsgProtocole::LOB_DUPLICATE_PROJECT_REP:{
            uint32_t projectId;
            std::string newName;
            *(event.data_packet_) >> projectId >> newName;
            ProjectData projet;

            projet.projectId = projectId;
            projet.projectName = newName;
            //Owner
            projet.role = 2;

            handleWindow_.addProjectToList(projet);
                
            break;
        }


        case MsgProtocole::LOB_CREATE_PROJECT_REP : {
            uint32_t newProjectId;
            *(event.data_packet_) >> newProjectId ;
            handleWindow_.updateCreatedProjectId(newProjectId);
            break;
        }


        case MsgProtocole::MAP_PUT_PIXELS_CIRCLE_REP : {
             
            break;
        }


        case MsgProtocole::MAP_PUT_PIXELS_CARRE_REP : {
            
            break;
        }


        case MsgProtocole::MAP_PUT_PIXELS_DIAM_REP : {
            
            break;
        }


        case MsgProtocole::MAP_ERASER_CIRCLE_REP : {
            
            break;
        }


        case MsgProtocole::MAP_ERASER_CARRE_REP : {
            
            break;
        }


        case MsgProtocole::MAP_ERASER_DIAM_REP : {
            
            break;
        }
    }        
}





