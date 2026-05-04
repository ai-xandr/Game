cmake_minimum_required(VERSION 3.11)
project(Game VERSION 1.0 LANGUAGES CXX)

# Toolchain Configuration
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# All source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp")

# Define output directory for build artifacts
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Dependencies (Conan)
find_package(SFML 3.0.2 COMPONENTS graphics window system REQUIRED)

# Target Definition
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(Game PRIVATE SFML::Graphics SFML::Window SFML::System)