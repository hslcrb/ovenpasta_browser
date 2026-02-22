cmake_minimum_required(VERSION 3.10)
project(ovenpasta)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
include_directories(src)
include_directories(include)
link_directories(${GTK3_LIBRARY_DIRS})

# Library sources (Engine)
file(GLOB_RECURSE LIB_SOURCES
    "src/html/*.cpp"
    "src/network/*.cpp"
    "src/render/*.cpp"
    "src/api/*.cpp"
)

# Application sources (CLI/App wrapper)
file(GLOB APP_SOURCES 
    "src/main.cpp"
    "src/ui/*.cpp"
)

# Build the embeddable library
add_library(ovenpasta_lib STATIC ${LIB_SOURCES})
target_link_libraries(ovenpasta_lib ${GTK3_LIBRARIES})

# Build the example web browser executable
add_executable(ovenpasta ${APP_SOURCES})
target_link_libraries(ovenpasta ovenpasta_lib ${GTK3_LIBRARIES})
