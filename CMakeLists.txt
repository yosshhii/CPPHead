cmake_minimum_required(VERSION 4.0)
project(CPPHead LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(PREPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/sfml")

add_executable(CPPHead main.cpp
		obstacle.h
		player.h)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)
target_link_libraries(CPPHead PRIVATE SFML::Graphics SFML::Window SFML::System)
