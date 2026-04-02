cmake_minimum_required(VERSION 4.0)
project(CPPHead LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(PREPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/sfml")

add_executable(CPPHead src/main.cpp
		src/game/obstacle.hpp
		src/game/player.hpp
		src/engine/window.hpp
		src/engine/window.cpp
		src/game/player.cpp
		src/game/background.hpp
		src/game/background.cpp)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)
target_link_libraries(CPPHead PRIVATE SFML::Graphics SFML::Window SFML::System)

add_custom_command(TARGET CPPHead POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/assets
		${CMAKE_BINARY_DIR}/assets
)