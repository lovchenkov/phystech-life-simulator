cmake_minimum_required(VERSION 3.15)
project(phystech-life-simulator)
set(CMAKE_CXX_STANDARD 17)

include_directories(include dist data)


file(GLOB SOURCES "src/*.cpp")
add_executable(main ${SOURCES} "dist/jsoncpp.cpp")

