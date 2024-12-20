cmake_minimum_required(VERSION 3.16)

set(CXX_STANDARD 14)
set(CXX_STANDARD_REQUIRED TRUE)

project(HothouseProject)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

find_package(Poco REQUIRED)
find_package(nlohmann_json REQUIRED)

file(GLOB_RECURSE INC source/*.hpp)
file(GLOB_RECURSE SRC source/*.cpp)

add_executable(server ${INC} ${SRC})

target_link_libraries(server PRIVATE Poco::Poco nlohmann_json::nlohmann_json)