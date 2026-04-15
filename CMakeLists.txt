cmake_minimum_required(VERSION 3.16)
project(Castles3)

set(CMAKE_CXX_STANDARD 17)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
endif()

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(Catch2 3 REQUIRED)

# Core static library (pure C++, zero SDL)
file(GLOB_RECURSE CORE_SOURCES src/core/*.cpp)
add_library(castles3_core STATIC ${CORE_SOURCES})
target_include_directories(castles3_core PUBLIC src/)
target_link_libraries(castles3_core nlohmann_json::nlohmann_json)

# Game executable
file(GLOB RENDERER_SOURCES src/renderer/*.cpp)
file(GLOB INPUT_SOURCES src/input/*.cpp)
add_executable(castle3
    src/main.cpp
    ${RENDERER_SOURCES}
    ${INPUT_SOURCES}
)
target_link_libraries(castle3
    castles3_core
    SDL2::SDL2
    SDL2_ttf::SDL2_ttf
    SDL2_mixer::SDL2_mixer
)

# Copy assets to build directory
if(EXISTS ${CMAKE_SOURCE_DIR}/data)
    file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR})
endif()
if(EXISTS ${CMAKE_SOURCE_DIR}/fonts)
    file(COPY ${CMAKE_SOURCE_DIR}/fonts DESTINATION ${CMAKE_BINARY_DIR})
endif()
if(EXISTS ${CMAKE_SOURCE_DIR}/music)
    file(COPY ${CMAKE_SOURCE_DIR}/music DESTINATION ${CMAKE_BINARY_DIR})
endif()

# Test executable
file(GLOB TEST_SOURCES tests/*.cpp)
add_executable(castles3_tests ${TEST_SOURCES})
target_include_directories(castles3_tests PRIVATE src/)
target_link_libraries(castles3_tests
    castles3_core
    Catch2::Catch2WithMain
)

enable_testing()
add_test(NAME castles3_tests COMMAND castles3_tests)
