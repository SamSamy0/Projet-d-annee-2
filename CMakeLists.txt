cmake_minimum_required(VERSION 3.16)
project(MyProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ajoute SFML (le code source que tu as mis dans lib/SFML)
add_subdirectory(lib/SFML)

add_executable(app
    src/main.cpp
)

target_link_libraries(app PRIVATE sfml-graphics sfml-window sfml-system)