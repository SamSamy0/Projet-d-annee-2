# 1. Toujours commencer par la version minimum
cmake_minimum_required(VERSION 3.16)

# 2. Déclarer le nom du projet
project(MonServeurReseau)

# 3. Options de compilation
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON) # Important pour Qt

# 4. Chercher les bibliothèques
find_package(Qt6 COMPONENTS Core Network Sql REQUIRED)
find_package(SFML 3 COMPONENTS Graphics Window System Network REQUIRED)
find_package(TGUI REQUIRED) 
find_package(Threads REQUIRED)

# 5. Définir l'exécutable (AJOUTER LA CIBLE)
# C'est ici qu'on crée "mon_serveur"
add_executable(mon_serveur 
    src/server/main.cpp
    src/server/worker.cpp
    src/server/servernetwork.cpp
    src/server/messagequeue.cpp
    src/server/reponsequeue.cpp      # <--- AJOUTÉ (pour ReponseQueue::pop, stop, etc.)
    src/server/databasemanager.cpp
    src/server/projectsmanager.cpp   # <--- AJOUTÉ (pour ProjectsManager)
    src/server/client.cpp            # <--- AJOUTÉ (pour Client::getAddress)
    src/server/messages.cpp          # <--- AJOUTÉ (pour MessageFactory)
    # Ajoutez ici tout autre .cpp présent dans src/server
)

# 6. Maintenant on peut lier des choses à "mon_serveur"
target_include_directories(mon_serveur PRIVATE src/server)

target_link_libraries(mon_serveur 
    PRIVATE 
    Qt6::Core 
    Qt6::Network
    Qt6::Sql
    SFML::Graphics 
    SFML::Window 
    SFML::System 
    SFML::Network
    tgui
    Threads::Threads
)