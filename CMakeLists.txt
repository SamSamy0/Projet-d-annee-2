cmake_minimum_required(VERSION 3.10)
project(Projet)
set(CMAKE_CXX_STANDARD 17)

set(SFML_STATIC_LIBRARIES ON)
find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System Network)

find_package(TGUI 1.0 REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Core Sql)



set(SOURCES
    main.cpp
    src/Window/Identifier.cpp
    src/Window/displayWindow.cpp
    src/server/clientnetwork.cpp
    src/server/handler.cpp
    src/project/map.cpp
    src/project/layer.cpp
    src/project/zoom.cpp
    src/project/move.cpp
    src/project/toolbar.cpp
)

add_executable(sfml-app ${SOURCES})

target_link_libraries(sfml-app 
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Network
    TGUI::TGUI
    Qt5::Core
    Qt5::Network
    Qt5::Sql
)
# set(SOURCES
#     main.cpp
#     src/Window/Identifier.cpp
#     src/Window/displayWindow.cpp
#     src/Window/displayWindow.hpp
#     # src/server/clientnetwork.cpp
#     # src/server/handler.cpp
#     # src/project/project.cpp
#     # src/project/tool.cpp
#     # src/project/map.cpp
#     # src/project/layer.cpp
#     # src/project/zoom.cpp
#     # src/project/move.cpp
#     # src/project/toolbar.cpp
#     
# )
