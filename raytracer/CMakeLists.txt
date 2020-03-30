cmake_minimum_required(VERSION 2.8.9)
project(raytracer)

#Bring the headers, such as Student.h into the project
include_directories(include)

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADER_FILES "include/*.h")

add_executable(raytracer ${SOURCES} ${HEADER_FILES})