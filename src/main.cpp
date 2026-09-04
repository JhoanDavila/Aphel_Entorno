#include <iostream>
#include <memory>

// Incluir Lector (AphluiReader)
#include "ui/lector_aphelui/reader/AphluiReader.h"

// Incluir Modulos del Renderer
#include "ui/Renderer/structurerender/StructureRender.h"
#include "ui/Renderer/paintrender/PaintRender.h"

int main() {
    std::cout << "=== [APHEL ENGINE UI - PIPELINE TEST] ===" << std::endl;

    // 1. CARGA Y PARSEO DEL ARCHIVO .aphlui -> AST
    std::string filePath = "../src/ui/Aphel_Ui/example.aphlui";
    std::cout << "\n[1/3] Cargando UI desde: " << filePath << std::endl;
    
    std::shared_ptr<Node> root = AphluiReader::loadFromFile(filePath);

    if (!root) {
        std::cerr << "[ERROR] Falló la carga del árbol AST. Revisa los errores de sintaxis." << std::endl;
        return -1;
    }

    std::cout << "[ÉXITO] Árbol AST cargado correctamente en memoria." << std::endl;

    // 2. CONSTRUCCIÓN DE GEOMETRÍA -> StructureRender
    std::cout << "\n[2/3] Generando vértices e índices (StructureRender)..." << std::endl;
    
    StructureRender structureRender;
    RenderBatch batch = structureRender.buildGeometry(root);

    if (batch.vertices.empty() || batch.indices.empty()) {
        std::cout << "[WARN] El batch está vacío. No hay nodos visuales para pintar." << std::endl;
        return 0;
    }

    // 3. PREPARACIÓN Y DIBUJADO DE LA GEOMETRÍA -> PaintRender
    std::cout << "\n[3/3] Evaluando integración con PaintRender..." << std::endl;

    PaintRender paintRender;

    // Nota: Si aún no tienes la ventana GLFW/SDL iniciada en main, 
    // puedes llamar a debugPrintGPUState para verificar el cálculo del lote
    paintRender.debugPrintGPUState(batch);

    /* 
    ===================================================================
    FLUJO COMPLETO EN EL BUCLE DE RENDER (Una vez iniciada la ventana GLFW/GLEW):
    ===================================================================
    
    if (!paintRender.init()) {
        std::cerr << "Error al inicializar Shaders y Buffers de OpenGL" << std::endl;
        return -1;
    }

    // Definir la proyección ortográfica con las dimensiones de tu ventana
    paintRender.setOrthographicProjection(800.0f, 600.0f);

    // En el Render Loop de GLFW:
    // while (!glfwWindowShouldClose(window)) {
    //     glClear(GL_COLOR_BUFFER_BIT);
    //     
    //     paintRender.render(batch);
    //     
    //     glfwSwapBuffers(window);
    //     glfwPollEvents();
    // }
    */

    std::cout << "\n=== [PIPELINE COMPLETADO EXITOSAMENTE] ===" << std::endl;
    return 0;
}