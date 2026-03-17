cmake_minimum_required(VERSION 3.16)
project(Castles2)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_executable(castles2 src/main.cpp src/ui.cpp)

target_link_libraries(castles2 SDL2::SDL2 SDL2_ttf::SDL2_ttf)