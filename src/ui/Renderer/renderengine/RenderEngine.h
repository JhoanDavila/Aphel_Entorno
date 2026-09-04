#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include <memory>
#include "ui/nodes/node/Node.h"
#include "ui/nodes/window/Window.h"
#include "ui/Renderer/windowso/WindowSO.h"

class RenderEngine {
private:
    WindowSO appWindow;

public:
    RenderEngine() = default;
    ~RenderEngine() = default;

    // Inicializa la ventana y valida que el nodo raíz sea una WindowNode
    bool init(const std::shared_ptr<Node>& rootNode);

    // Inicia el bucle principal de renderizado
    void run(const std::shared_ptr<Node>& rootNode);

    // Detiene el motor y libera recursos
    void stop();
};

#endif // RENDER_ENGINE_H