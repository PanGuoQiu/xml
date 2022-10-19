cmake_minimum_required(VERSION 3.10)

project(pan)

set(CMAKE_CXX_STANDARD 17)

include_directories(xml/)
file(GLOB XML xml/Xml.cpp xml/Parser.cpp)

add_executable(pan main.cpp ${XML})