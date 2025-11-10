cmake_minimum_required(VERSION 3.10)
project(paint)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/glad.c
    src/plano.cpp
    src/forma.cpp
    src/triangulo.cpp
    src/retangulo.cpp
    src/circulo.cpp
    src/colorpicker.cpp
    src/poligono.cpp
)
target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME}
    glfw
    ${OPENGL_LIBRARIES}
    glm::glm               
)