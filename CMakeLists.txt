cmake_minimum_required(VERSION 3.10)
project(aphel)

set(CMAKE_CXX_STANDARD 17)

# 1. Buscar librerías requeridas (GLEW y OpenGL)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

# Include directories
include_directories(
    src
    ${GLEW_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR}
)

# 2. Registrar todos los archivos fuente
set(SOURCES
    src/main.cpp
    
    # Lector
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
    src/ui/Renderer/structurerender/StructureRender.cpp
)

add_executable(aphel ${SOURCES})

# 3. Enlazar las librerías al ejecutable (Esto resuelve los errores __glew*)
target_link_libraries(aphel
    PRIVATE
    GLEW::GLEW
    OpenGL::GL
)