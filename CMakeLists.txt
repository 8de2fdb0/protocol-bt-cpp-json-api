cmake_minimum_required(VERSION 3.14)
project(jura-api-server VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# FetchContent for dependencies
include(FetchContent)

# Fetch nlohmann_json for JSON handling
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)

# Fetch httplib for HTTP server
FetchContent_Declare(
    httplib
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
    GIT_TAG v0.14.3
)

FetchContent_MakeAvailable(nlohmann_json httplib)

# Source files
set(SOURCES
    src/main.cpp
    src/api_server.cpp
    src/jura_controller.cpp
    src/jura_protocol.cpp
)

set(HEADERS
    src/api_server.h
    src/jura_controller.h
    src/jura_protocol.h
)

# Executable
add_executable(jura-api-server ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(jura-api-server 
    PRIVATE 
    nlohmann_json::nlohmann_json
    httplib::httplib
)

# Include directories
target_include_directories(jura-api-server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Installation
install(TARGETS jura-api-server DESTINATION bin)
