cmake_minimum_required(VERSION 3.10)
project(MonProjetSFML)

# On force le C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# On cherche les bibliothèques sur le système
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
find_package(TGUI REQUIRED)

# Liste de TOUS tes fichiers .cpp (ajoute les nouveaux ici au fur et à mesure)
set(SOURCES
    main.cpp
    src/displayWindow.cpp
    src/displayWindow.hpp
    src/server/clientnetwork.cpp
    src/server/handler.cpp
    # src/server/protocol.hpp
)

# Création de l'exécutable
add_executable(sfml-app ${SOURCES})

# On lie les bibliothèques à l'exécutable
target_link_libraries(sfml-app sfml-graphics sfml-window sfml-system tgui)

# Pour que le compilateur trouve tes .hpp dans src/serveur
target_include_directories(sfml-app PRIVATE . src/server)
