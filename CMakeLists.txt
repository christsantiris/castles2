cmake_minimum_required(VERSION 3.16)
project(Castles3)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(SDL2_mixer REQUIRED)

file(GLOB SOURCES 
    src/*.cpp
    src/ui/*.cpp
    src/ai/*.cpp
    src/engine/*.cpp
    src/geography/*.cpp
    src/military/*.cpp
    src/resources/*.cpp
)

add_executable(castles3 ${SOURCES})

file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR}) # add data folder to build directory
file(COPY ${CMAKE_SOURCE_DIR}/fonts DESTINATION ${CMAKE_BINARY_DIR}) # add fonts to build directory
file(COPY ${CMAKE_SOURCE_DIR}/music DESTINATION ${CMAKE_BINARY_DIR}) # add music to build directory

target_link_libraries(castles3 SDL2::SDL2 SDL2_ttf::SDL2_ttf nlohmann_json::nlohmann_json SDL2_mixer::SDL2_mixer)
