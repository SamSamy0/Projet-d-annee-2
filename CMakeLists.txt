cmake_minimum_required(VERSION 3.10)
project(ProjetCollaboratif)

set(CMAKE_CXX_STANDARD 17)

set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_TESTS OFF CACHE BOOL "" FORCE)

add_subdirectory(lib/SFML-master/SFML-master)

set(SOURCES
    main.cpp
    src/Window/Identifier.cpp
    src/Window/displayWindow.cpp
    src/Window/displayWindow.hpp
    src/server/clientnetwork.cpp
    src/server/handler.cpp
)
add_executable(sfml-app ${SOURCES})
target_link_libraries(sfml-app 
    sfml-graphics 
    sfml-window 
    sfml-system 
    sfml-network
    tgui
)
target_include_directories(sfml-app PRIVATE . src/server)
