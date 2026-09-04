cmake_minimum_required(VERSION 3.16)
project(aphel CXX)

# Usar estándar C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Buscar dinámicamente todos los archivos .cpp dentro de src/
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Añadir la carpeta 'src' al path de includes para usar rutas como:
# #include "ui/lector_aphelui/reader/AphluiReader.h"
include_directories(src)

# Crear el ejecutable del proyecto
add_executable(aphel ${SOURCES})

# Buscar y enlazar librerías gráficas (OpenGL, GLFW, GLEW) para cuando integremos el Renderer
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

target_link_libraries(aphel PRIVATE OpenGL::GL glfw GLEW::GLEW)