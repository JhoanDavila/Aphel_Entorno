#include "WindowSO.h"
#include <iostream>

WindowSO::WindowSO() : window(nullptr) {}

WindowSO::~WindowSO() {
    close();
}

bool WindowSO::init(const std::shared_ptr<WindowNode>& windowNode) {
    if (!windowNode) {
        std::cerr << "[WindowSO Error] El nodo de ventana provisto es nulo." << std::endl;
        return false;
    }

    if (!glfwInit()) {
        std::cerr << "[WindowSO Error] Fallo al inicializar GLFW." << std::endl;
        return false;
    }

    int width = windowNode->size.width > 0 ? windowNode->size.width : 800;
    int height = windowNode->size.height > 0 ? windowNode->size.height : 600;
    std::string title = !windowNode->title.empty() ? windowNode->title : "AphelUI Window";

    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!window) {
        std::cerr << "[WindowSO Error] No se pudo crear la ventana nativa de GLFW." << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    // =========================================================================
    // AQUÍ ESTÁ LA ADICIÓN: Ajustar el viewport al tamaño real del framebuffer
    // =========================================================================
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);
    // =========================================================================

    if (glewInit() != GLEW_OK) {
        std::cerr << "[WindowSO Error] Fallo al inicializar GLEW." << std::endl;
        return false;
    }

    if (!painter.init()) {
        std::cerr << "[WindowSO Error] Fallo al inicializar los Shaders de PaintRender." << std::endl;
        return false;
    }

    // Mantenemos la proyección usando el tamaño lógico del nodo
    painter.setOrthographicProjection(static_cast<float>(width), static_cast<float>(height));

    return true;
}

void WindowSO::startLoop(const std::shared_ptr<Node>& rootNode) {
    if (!window) return;

    // Intentar obtener el color de fondo definido en el nodo WindowNode
    auto winNode = std::dynamic_pointer_cast<WindowNode>(rootNode);

    while (!glfwWindowShouldClose(window)) {
        // Limpiar pantalla usando el color del nodo (convertido a float 0.0 - 1.0)
        if (winNode) {
            glClearColor(
                winNode->backgroundColor.rFloat(),
                winNode->backgroundColor.gFloat(),
                winNode->backgroundColor.bFloat(),
                winNode->backgroundColor.aFloat()
            );
        } else {
            glClearColor(0.2f, 0.2f, 0.2f, 1.0f); // Color gris por defecto
        }
        glClear(GL_COLOR_BUFFER_BIT);

        // A. Convertir el árbol de nodos en vértices e índices
        RenderBatch batch = structure.buildGeometry(rootNode);

        // B. Enviar la geometría a la GPU
        painter.render(batch);

        // Intercambiar buffers y procesar eventos de entrada
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void WindowSO::close() {
    painter.cleanup();
    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    glfwTerminate();
}