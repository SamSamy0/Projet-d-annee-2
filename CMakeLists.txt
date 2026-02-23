cmake_minimum_required(VERSION 3.16)
project(Projet)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POLICY_VERSION_MINIMUM "3.5" CACHE STRING "Minimum CMake policy" FORCE)
include(FetchContent)


set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries" FORCE)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)


# 1. Téléchargement et compilation de la SFML 3.0.0
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        3.0.0
)
FetchContent_MakeAvailable(SFML)

# 2. Téléchargement et configuration de TGUI 
FetchContent_Declare(
    TGUI
    GIT_REPOSITORY https://github.com/texus/TGUI.git
    GIT_TAG        1.x
)
set(TGUI_BACKEND SFML_GRAPHICS CACHE STRING "TGUI backend" FORCE)
set(TGUI_SHARED_LIBS ON CACHE BOOL "Build TGUI as a shared library" FORCE)
set(TGUI_BUILD_FRAMEWORK OFF CACHE BOOL "Build TGUI as a framework on macOS" FORCE)
FetchContent_MakeAvailable(TGUI)

set(SOURCES
    main.cpp
    src/Window/Identifier.cpp
    src/Window/displayWindow.cpp
    src/client/clientnetwork.cpp
    src/client/handler.cpp
    src/server/protocol.hpp
    src/project/map.cpp
    src/project/project.cpp
    src/project/layer.cpp
    src/project/zoom.cpp
    src/project/move.cpp
    src/project/toolbar.cpp
    src/project/tool.cpp
)

# 1. Ajoute ces lignes JUSTE AVANT ton add_executable
find_package(PkgConfig REQUIRED)
pkg_check_modules(HARFBUZZ REQUIRED harfbuzz)

# 2. Ton add_executable reste inchangé
add_executable(sfml-app ${SOURCES})

# 3. Modifie le target_link_libraries pour inclure les libs HarfBuzz
target_link_libraries(sfml-app PRIVATE
    SFML::Graphics 
    SFML::Window 
    SFML::System 
    SFML::Network
    TGUI::TGUI
    ${HARFBUZZ_LIBRARIES}  # <-- AJOUTE CETTE VARIABLE ICI
)
