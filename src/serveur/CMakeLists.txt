cmake_minimum_required(VERSION 3.16)
project(Server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Configuration Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql Network)

# 2. Chemins SFML 3.1 (On utilise ton installation PROPRE dans le home)
set(SFML_ROOT "$ENV{HOME}/sfml3_install")
set(SFML_INCLUDE_DIR "${SFML_ROOT}/include")
set(SFML_LIB_DIR "${SFML_ROOT}/lib")

# 3. Définition de l'exécutable
# J'ai ajouté l'include pour que tes headers dans message/ ou reponse/ soient trouvés
add_executable(Server 
    main.cpp 
    worker.cpp 
    message/message.cpp
    datamanager/databasemanager.cpp
    datamanager/projectsmanager.cpp
    servernetwork.cpp
    reponse/reponse.cpp
)

# 4. Inclusion des dossiers
target_include_directories(Server PRIVATE 
    ${SFML_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR} # Pour trouver message/message.hpp etc.
)

# 5. Liens vers les bibliothèques (On utilise les .so qu'on a compilés)
target_link_libraries(Server
    PRIVATE
        Qt6::Sql
        Qt6::Core
        ${SFML_LIB_DIR}/libsfml-network.so
        ${SFML_LIB_DIR}/libsfml-system.so
        pthread
        dl
)