CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(tripleprov)

add_executable(tripleprov src/diplodocus.cpp src/API.cpp src/BTCprov.cpp src/WDCqueries.cpp src/Client.cpp src/Server.cpp src/Conductor.cpp src/ConductorNode.cpp src/Functions.cpp src/KeyManager.cpp src/Molecules.cpp src/PageManager.cpp src/Partitioner.cpp src/TemplateManager.cpp src/TypesHierarchy.cpp src/TypesManager.cpp )
target_link_libraries (tripleprov)

install(TARGETS tripleprov RUNTIME DESTINATION bin)


set(CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_CXX_FLAGS "-Wall -std=c++11")
SET(CMAKE_CXX_FLAGS_DEBUG  "-g3 -O0 -ggdb")
