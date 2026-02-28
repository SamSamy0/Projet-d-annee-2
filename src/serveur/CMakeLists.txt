cmake_minimum_required(VERSION 3.16)
project(GimpServer)

# Standard C++17 requis pour SFML 3
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Configuration automatique de Qt (indispensable pour les Q_OBJECT)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 2. Recherche de Qt6 avec les modules dont tu as besoin
find_package(Qt6 REQUIRED COMPONENTS Widgets Sql Network)

# 3. Chemins manuels SFML 3.1 (On bypass le fichier config qui bug)
set(SFML_INCLUDE_DIR "/usr/local/include")
set(SFML_LIB_DIR "/usr/local/lib")

# 4. Définition de l'exécutable avec TOUS tes fichiers
add_executable(GimpServer 
    main.cpp 
    worker.cpp 
    message.cpp
    databasemanager.cpp
    projectsmanager.cpp
)

# 5. Inclusion des dossiers de headers
target_include_directories(GimpServer PRIVATE ${SFML_INCLUDE_DIR})

# 6. Liens vers les bibliothèques
target_link_libraries(GimpServer
    PRIVATE
        Qt6::Widgets
        Qt6::Sql
        Qt6::Network
        ${SFML_LIB_DIR}/libsfml-network-s.a
        ${SFML_LIB_DIR}/libsfml-system-s.a
        pthread
        dl
)