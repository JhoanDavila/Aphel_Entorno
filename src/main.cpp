#include <iostream>
#include <memory>

#include "ui/lector_aphelui/reader/AphluiReader.h"
#include "ui/nodes/window/Window.h"
#include "ui/Renderer/windowso/WindowSO.h"

int main(int argc, char* argv[]) {
    // 1. Determinar la ruta del archivo .aphlui
    std::string filePath = "/home/jhoan/personal_deloved/Aphel_Entorno/src/ui/Aphel_Ui/example.aphlui";
    
    if (argc > 1) {
        filePath = argv[1];
    }

    std::cout << "[AphelUI] Cargando archivo de interfaz: " << filePath << "..." << std::endl;

    // 2. Cargar y procesar el archivo .aphlui (Cleaner -> Tokenizer -> Parser)
    std::shared_ptr<Node> rootNode = AphluiReader::loadFromFile(filePath);

    if (!rootNode) {
        std::cerr << "[AphelUI Fatal Error] No se pudo construir el árbol de nodos. "
                  << "Verifica los errores de sintaxis o la ruta del archivo." << std::endl;
        return -1;
    }

    // 3. Validar que la raíz sea de tipo WindowNode
    auto windowNode = std::dynamic_pointer_cast<WindowNode>(rootNode);
    if (!windowNode) {
        std::cerr << "[AphelUI Fatal Error] El nodo raíz debe ser de tipo 'Window'." << std::endl;
        return -1;
    }

    std::cout << "[AphelUI] Árbol de nodos generado con éxito." << std::endl;
    std::cout << "          - Título de ventana: " << windowNode->title << std::endl;
    std::cout << "          - Dimensiones: " << windowNode->size.width << "x" << windowNode->size.height << std::endl;

    // 4. Inicializar la ventana del SO (GLFW + OpenGL + PaintRender)
    WindowSO appWindow;

    if (!appWindow.init(windowNode)) {
        std::cerr << "[AphelUI Fatal Error] Fallo al inicializar la ventana de GLFW/OpenGL." << std::endl;
        return -1;
    }

    // 5. Iniciar el bucle principal de renderizado
    std::cout << "[AphelUI] Iniciando bucle de renderizado..." << std::endl;
    appWindow.startLoop(rootNode);

    // 6. Cierre limpio
    appWindow.close();
    std::cout << "[AphelUI] Aplicación finalizada correctamente." << std::endl;

    return 0;
}