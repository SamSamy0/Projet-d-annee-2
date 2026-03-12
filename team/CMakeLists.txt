cmake_minimum_required(VERSION 3.16)
project(MyProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt5 REQUIRED COMPONENTS Core Sql)

add_executable(app src/main.cpp)

target_link_libraries(app Qt5::Core Qt5::Sql)
