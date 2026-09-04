#include <iostream>
#include "ui/lector_aphelui/reader/AphluiReader.h"
#include "ui/Renderer/structurerender/StructureRender.h"

int main() {
    // 1. Cargar el árbol con tu AphluiReader
    auto root = AphluiReader::loadFromFile("../src/ui/Aphel_Ui/example.aphlui");

    if (!root) {
        std::cerr << "Error al cargar el archivo .aphlui" << std::endl;
        return -1;
    }

    std::cout << "Árbol cargado exitosamente." << std::endl;

    // 2. Procesar la geometría con StructureRender
    StructureRender structureRender;
    RenderBatch batch = structureRender.buildGeometry(root);

    // 3. Imprimir el resultado para depurar
    structureRender.debugPrintBatch(batch);

    return 0;
}