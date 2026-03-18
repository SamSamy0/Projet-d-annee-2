cmake_minimum_required(VERSION 3.16)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(FetchContent)

project(Projet)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries" FORCE)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

# ── macOS spécifique ──────────────────────────────────────────────
if(APPLE)
    set(CMAKE_MACOSX_RPATH ON)
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)
    # Cherche Qt6 installé via Homebrew
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt6")
    list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/qt6")
endif()

# ── SFML ─────────────────────────────────────────────────────────
find_package(SFML 3.0 COMPONENTS Graphics Window System Network QUIET)

if(NOT SFML_FOUND)
    message(STATUS "SFML 3.0 non trouvée. Téléchargement...")
    FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG        3.0.0
        GIT_SHALLOW    TRUE  # plus rapide
    )
    FetchContent_MakeAvailable(SFML)
else()
    message(STATUS "SFML 3.0 trouvée localement.")
endif()

# ── TGUI ─────────────────────────────────────────────────────────
find_package(TGUI 1.0 QUIET)
if(NOT TGUI_FOUND)
    message(STATUS "TGUI non trouvé. Téléchargement...")
    FetchContent_Declare(
        TGUI
        GIT_REPOSITORY https://github.com/texus/TGUI.git
        GIT_TAG        1.x
        GIT_SHALLOW    TRUE
    )
    set(TGUI_BACKEND SFML_GRAPHICS CACHE STRING "TGUI backend" FORCE)
    set(TGUI_SHARED_LIBS ON CACHE BOOL "" FORCE)
    set(TGUI_BUILD_FRAMEWORK OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(TGUI)
else()
    message(STATUS "TGUI trouvé localement.")
endif()

# ── Qt6 ──────────────────────────────────────────────────────────
find_package(Qt6 REQUIRED COMPONENTS Core Sql Gui)  # ← Core ajouté obligatoire
set(CMAKE_AUTOMOC ON)  # ← nécessaire pour Qt

# ── CLIENT ───────────────────────────────────────────────────────
set(SOURCES
    ../src/client/client.cpp
    ../src/Window/Window.cpp
    ../src/Window/windowLogin.cpp
    ../src/Window/windowMenu.cpp
    ../src/Window/projectWidgets/windowGame.cpp
    ../src/Window/projectWidgets/toolBarWidgets.cpp
    ../src/Window/projectWidgets/layerWidgets.cpp
    ../src/Window/projectWidgets/chatWidgets.cpp
    ../src/Window/projectWidgets/toolbarOptionsWidgets/penOptions.cpp
    ../src/Window/projectWidgets/toolbarOptionsWidgets/eraserOptions.cpp
    ../src/Window/projectWidgets/toolbarOptionsWidgets/spriteBrushOptions.cpp
    ../src/client/clientnetwork.cpp
    ../src/client/handler.cpp
    ../src/client/receiverwindow.cpp
    ../src/project/map.cpp
    ../src/project/move.cpp
    ../src/project/project.cpp
    ../src/project/toolbar.cpp
    ../src/project/zoom.cpp
    ../src/project/assetmanager.cpp
    ../src/project/Tool/brush.cpp
    ../src/project/Tool/tool.cpp
    ../src/project/Tool/shift.cpp
    ../src/project/Tool/nonetool.cpp
    ../src/project/Tool/pixelbrush.cpp
    ../src/project/Tool/pixelshift.cpp
    ../src/project/Tool/spritebrush.cpp
    ../src/project/Tool/spriteshift.cpp
    ../src/project/Tool/spriteeraser.cpp
    ../src/project/Tool/spriteselection.cpp
    ../src/project/Layer/layer.cpp
    ../src/project/Layer/pixellayer.cpp
    ../src/project/Layer/spritelayer.cpp
    ../src/project/Chat/chat.cpp
    ../src/project/Chat/userMessage.cpp
    ../src/project/Chat/systemNotification.cpp
    ../src/project/Chat/date.cpp
    ../src/common/protocol.cpp
)

add_executable(client ${SOURCES})

target_link_libraries(client PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Network
    TGUI::TGUI
    Qt6::Core   # ← ajouté
    Qt6::Sql
)

# Fix RPATH sur macOS pour trouver les .dylib
if(APPLE)
    set_target_properties(client PROPERTIES
        BUILD_RPATH "@loader_path"
        INSTALL_RPATH "@loader_path"
    )
endif()

# ── SERVEUR ──────────────────────────────────────────────────────
set(SERVER_SOURCES
    ../src/server/client.cpp
    ../src/server/datamanager/databasemanager.cpp
    ../src/server/main.cpp
    ../src/server/message/message.cpp
    ../src/server/datamanager/projectsmanager.cpp
    ../src/server/reponse/reponse.cpp
    ../src/common/protocol.cpp
    ../src/server/servernetwork.cpp
    ../src/server/worker.cpp
)

add_executable(server ${SERVER_SOURCES})

target_link_libraries(server PRIVATE
    SFML::System
    SFML::Network
    Qt6::Core   # ← ajouté
    Qt6::Sql
    Qt6::Gui
)

if(APPLE)
    set_target_properties(server PROPERTIES
        BUILD_RPATH "@loader_path"
        INSTALL_RPATH "@loader_path"
    )
endif()
