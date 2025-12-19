cmake_minimum_required(VERSION 3.20)

project(
    PenguinScan
    VERSION 0.1.0
    LANGUAGES CXX
)

# -----------------------------
# C++ standard configuration
# -----------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# -----------------------------
# Executable target
# -----------------------------
add_executable(PenguinScan)

# -----------------------------
# Source files
# -----------------------------
target_sources(PenguinScan
    PRIVATE
        src/main.cpp
 "include/cli/ArgParser.h" "src/cli/ArgParser.cpp" "include/net/SocketContext.h" "src/net/SocketContext.cpp" "include/net/TcpSocket.h" "src/net/TcpSocket.cpp" "include/engine/ScanningEngine.h" "src/engine/ScanningEngine.cpp" "include/net/ScanResult.h" "include/net/Platform.h")

# -----------------------------
# Include directories
# -----------------------------
target_include_directories(PenguinScan
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

# -----------------------------
# Platform-specific settings
# -----------------------------
if (WIN32)
    target_link_libraries(PenguinScan PRIVATE ws2_32)
endif()

# -----------------------------
# Warnings
# -----------------------------
if (MSVC)
    target_compile_options(PenguinScan PRIVATE /W4 /permissive-)
else()
    target_compile_options(PenguinScan PRIVATE -Wall -Wextra -Wpedantic)
endif()
