cmake_minimum_required(VERSION 3.10)
project(ovenpasta)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
include_directories(src)
link_directories(${GTK3_LIBRARY_DIRS})

# Collect source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(ovenpasta ${SOURCES})
target_link_libraries(ovenpasta ${GTK3_LIBRARIES})
