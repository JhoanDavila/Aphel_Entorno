#ifndef WINDOW_SO_H
#define WINDOW_SO_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <string>

#include "ui/nodes/window/Window.h"
#include "ui/Renderer/structurerender/StructureRender.h"
#include "ui/Renderer/paintrender/PaintRender.h"

class WindowSO {
private:
    GLFWwindow* window;
    PaintRender painter;
    StructureRender structure;

public:
    WindowSO();
    ~WindowSO();

    // Inicializa la ventana nativa leyendo las propiedades del nodo WindowNode
    bool init(const std::shared_ptr<WindowNode>& windowNode);

    // Inicia el bucle de eventos y dibujado de la aplicación
    void startLoop(const std::shared_ptr<Node>& rootNode);

    // Libera la ventana y finaliza el contexto del sistema operativo
    void close();
};

#endif // WINDOW_SO_H