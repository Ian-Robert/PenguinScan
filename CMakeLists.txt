cmake_minimum_required(VERSION 3.20)

project(
    PenguinScan
    VERSION 1.0.0
    LANGUAGES CXX
)

# C++ Standard 
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Find Threads (REQUIRED for Linux)
set(CMAKE_THREAD_PREFER_PTHREAD ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Executable target
add_executable(PenguinScan)

# Source files 
target_sources(PenguinScan
    PRIVATE
        src/main.cpp
        
        # CLI
        src/cli/ArgParser.cpp
        include/cli/ArgParser.h
        
        # Networking
        src/net/SocketContext.cpp
        include/net/SocketContext.h
        src/net/TcpSocket.cpp
        include/net/TcpSocket.h
        include/net/Platform.h
        include/net/ScanResult.h
        
        # Engine
        src/engine/ScanningEngine.cpp
        include/engine/ScanningEngine.h
        include/ScanTypes.h

        # Utilities
        src/util/IpUtils.cpp
        include/util/IpUtils.h
        src/util/Reporter.cpp
        include/util/Reporter.h
)


# Include directories
target_include_directories(PenguinScan
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)


# Linking Libraries
# Link Threads (Works for both Linux -lpthread and Windows)
target_link_libraries(PenguinScan PRIVATE Threads::Threads)

if (WIN32)
    target_link_libraries(PenguinScan PRIVATE ws2_32)
endif()


# Compiler Warnings
if (MSVC)
    target_compile_options(PenguinScan PRIVATE /W4 /permissive-)
else()
    target_compile_options(PenguinScan PRIVATE -Wall -Wextra -Wpedantic)
endif()