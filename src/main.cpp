#include <iostream>
#include "ui/lector_aphelui/reader/AphluiReader.h"
#include "ui/Renderer/renderengine/RenderEngine.h"

int main(int argc, char* argv[]) {
    std::string filePath = "/home/jhoan/personal_deloved/Aphel_Entorno/src/ui/Aphel_Ui/example.aphlui";
    if (argc > 1) filePath = argv[1];

    // 1. Cargar el árbol de nodos (Trabajo del Lector)
    auto rootNode = AphluiReader::loadFromFile(filePath);
    if (!rootNode) return -1;

    // 2. Administrar el renderizado (Trabajo del RenderEngine)
    RenderEngine engine;
    if (engine.init(rootNode)) {
        engine.run(rootNode);
        engine.stop();
    }

    return 0;
}