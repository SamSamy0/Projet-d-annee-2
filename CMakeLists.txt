cmake_minimum_required(VERSION 3.10)
project(Projet)
set(CMAKE_CXX_STANDARD 17)

find_package(SFML 3.0 COMPONENTS Graphics Window System Network REQUIRED)
find_package(TGUI 1.0 REQUIRED)

set(SOURCES
    main.cpp
    src/Window/Identifier.cpp
    src/Window/displayWindow.cpp
    src/server/clientnetwork.cpp
    src/server/handler.cpp
    src/project/map.cpp
    src/project/project.cpp
    src/project/layer.cpp
    src/project/zoom.cpp
    src/project/move.cpp
    src/project/toolbar.cpp
    src/project/tool.cpp
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
add_executable(sfml-app ${SOURCES})

target_link_libraries(sfml-app 
    SFML::Graphics 
    SFML::Window 
    SFML::System 
    SFML::Network
    TGUI::TGUI
)
