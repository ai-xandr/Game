cmake_minimum_required(VERSION 3.11)
project(Game VERSION 1.0 LANGUAGES CXX)

# --- Toolchain Configuration ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- All files ---
set(SOURCES 
    src/main.cpp 
)

# Define output directory for build artifacts
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# --- Dependency Management (SFML) ---
include(FetchContent)

# Fetching SFML directly from source to ensure binary compatibility
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        2.6.1
)

# Disable unneeded modules to reduce compilation time and dependencies
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(SFML_INSTALL_PDB OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(SFML)

# --- Target Definition ---
add_executable(${PROJECT_NAME} ${SOURCES})

# Link against SFML targets provided by FetchContent
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        sfml-graphics 
        sfml-window 
        sfml-system
)

# --- Git Automation ---
set(HOOK_SRC "${CMAKE_SOURCE_DIR}/scripts/hooks/commit-msg")
set(HOOK_DST "${CMAKE_SOURCE_DIR}/.git/hooks/commit-msg")

if(EXISTS "${HOOK_SRC}")
    # Bootstrap project environment by installing local git hooks
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${HOOK_SRC}" "${HOOK_DST}"
        COMMENT "Deploying git hooks..."
    )
    
    if(UNIX)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND chmod +x "${HOOK_DST}"
        )
    endif()
endif()
