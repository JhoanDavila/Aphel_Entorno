cmake_minimum_required(VERSION 3.10)
project(AphelEngine VERSION 0.0.0.6 LANGUAGES CXX)

# Usamos C++17 como estándar
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Añadimos la carpeta 'src' para resolver includes fácilmente
include_directories(src)

# Recopilamos todos los archivos fuente dentro de src
file(GLOB_RECURSE SOURCES 
    "src/main.cpp"
    "src/ui/nodes/node/*.cpp"
    "src/ui/lector_aphelui/cleaner/*.cpp"
    "src/ui/lector_aphelui/tokenizer/*.cpp"
    "src/ui/lector_aphelui/parser/*.cpp"
)

# Generamos el ejecutable ejecutable 'aphel'
add_executable(aphel ${SOURCES})