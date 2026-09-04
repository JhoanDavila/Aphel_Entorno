cmake_minimum_required(VERSION 3.10)
project(aphel)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Buscar librerías requeridas (GLEW, OpenGL y GLFW)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# Directorios de cabecera
include_directories(
    src
    ${GLEW_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR}
)

# 2. Registrar todos los archivos fuente (.cpp)
# 2. Registrar todos los archivos fuente (.cpp)
# 2. Registrar todos los archivos fuente (.cpp)
set(SOURCES
    src/main.cpp
    
    # Lector AphelG
    src/Engine/lector_aphelg/Cleaner/Cleaner.cpp
    src/Engine/lector_aphelg/ErrorManager/ErrorManager.cpp
    src/Engine/lector_aphelg/Tokenizer/Tokenizer.cpp
    src/Engine/lector_aphelg/Parser/Parser.cpp # <-- AÑADIDO
    src/Engine/lector_aphelg/PathResolver/PathResolver.cpp

    # Lector AphelUI
    src/ui/lector_aphelui/cleaner/Cleaner.cpp
    src/ui/lector_aphelui/error_manager/ErrorManager.cpp
    src/ui/lector_aphelui/parser/Parser.cpp
    src/ui/lector_aphelui/reader/AphluiReader.cpp
    src/ui/lector_aphelui/tokenizer/Tokenizer.cpp
    
    # Nodos
    src/ui/nodes/2dspace/2DSpace.cpp
    src/ui/nodes/node/Node.cpp
    src/ui/nodes/visual_node/VisualNode.cpp
    src/ui/nodes/window/Window.cpp
    
    # Renderer
    src/ui/Renderer/paintrender/PaintRender.cpp
    src/ui/Renderer/renderengine/RenderEngine.cpp
    src/ui/Renderer/structurerender/StructureRender.cpp
    src/ui/Renderer/windowso/WindowSO.cpp
)

# 3. Definir ejecutable
add_executable(aphel ${SOURCES})

# 4. Enlazar dependencias al ejecutable
target_link_libraries(aphel
    PRIVATE
    GLEW::GLEW
    OpenGL::GL
    glfw
)