cmake_minimum_required(VERSION 2.8.9)
project(raytracer)

#glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

#opengl 
find_package(OpenGL REQUIRED)

add_subdirectory(dependencies/glfw-3.3.2/glfw-3.3.2)
add_subdirectory(dependencies/glad/)

#Bring the headers, such as Student.h into the project
include_directories(include)

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADER_FILES "include/*.h")

add_executable(raytracer ${SOURCES} ${HEADER_FILES})

target_link_libraries(raytracer glfw)
target_link_libraries(raytracer PRIVATE glad)