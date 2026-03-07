include(FetchContent)
cmake_minimum_required(VERSION 3.16)
project(Projet)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POLICY_VERSION_MINIMUM "3.5" CACHE STRING "Minimum CMake policy" FORCE)


set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries" FORCE)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin) #on met l'executable dans le dossier bin

#-------------------------------------------SFML-------------------------------------------
find_package(SFML 3.0 COMPONENTS Graphics Window System Network QUIET)

if(NOT SFML_FOUND)
    message(STATUS "SFML 3.0 non trouvée sur le système. Téléchargement via FetchContent...")
    FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG        3.0.0
    )
    FetchContent_MakeAvailable(SFML)
else()
    message(STATUS "SFML 3.0 trouvée localement. Téléchargement ignoré.")
endif()

#------------------------------------------------------------------------------------------

#-------------------------------------------TGUI-------------------------------------------
find_package(TGUI 1.0 QUIET)
if(NOT TGUI_FOUND)
    message(STATUS "TGUI non trouvé sur le système. Téléchargement via FetchContent...")
    FetchContent_Declare(
        TGUI
        GIT_REPOSITORY https://github.com/texus/TGUI.git
        GIT_TAG        1.x
    )
    set(TGUI_BACKEND SFML_GRAPHICS CACHE STRING "TGUI backend" FORCE)
    set(TGUI_SHARED_LIBS ON CACHE BOOL "Build TGUI as a shared library" FORCE)
    set(TGUI_BUILD_FRAMEWORK OFF CACHE BOOL "Build TGUI as a framework on macOS" FORCE)
    FetchContent_MakeAvailable(TGUI)
else()
    message(STATUS "TGUI trouvé localement. Téléchargement ignoré.")
endif()

#------------------------------------------------------------------------------------------



set(SOURCES
    ../src/client/client.cpp
    ../src/Window/Window.cpp
    ../src/Window/windowGame.cpp
    ../src/Window/windowLogin.cpp
    ../src/Window/windowMenu.cpp
    ../src/server/clientnetwork.cpp
    ../src/server/handler.cpp
    ../src/server/protocol.hpp
    ../src/project/map.cpp
    ../src/project/project.cpp
    ../src/project/zoom.cpp
    ../src/project/move.cpp
    ../src/project/toolbar.cpp
    ../src/project/Tool/tool.cpp
    ../src/project/Tool/brush.cpp
    ../src/project/Tool/pixelbrush.cpp
    ../src/project/Tool/shift.cpp
    ../src/project/Tool/spriteshift.cpp
    ../src/project/Tool/pixelshift.cpp
    ../src/project/Layer/layer.cpp
    ../src/project/Layer/pixellayer.cpp
    ../src/project/Layer/spritelayer.cpp
    
)

add_executable(client ${SOURCES})

target_link_libraries(client PRIVATE
    SFML::Graphics 
    SFML::Window 
    SFML::System 
    SFML::Network
    TGUI::TGUI
)

