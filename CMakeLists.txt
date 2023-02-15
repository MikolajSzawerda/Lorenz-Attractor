cmake_minimum_required(VERSION 3.24)
project(Lorenz)

set(CMAKE_CXX_STANDARD 17)
set(OpenGlLinkers -lglut -lGLU -lGL) # setting all the Glut libraries as one variable.

add_executable(Lorenz main.cpp)

target_link_libraries(Lorenz ${OpenGlLinkers}) # linking opengl libraries to the project
