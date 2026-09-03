#include <iostream>
#include <memory>

#include "ui/nodes/node/Node.h"
#include "ui/nodes/visual_node/VisualNode.h"
#include "ui/nodes/2dspace/2DSpace.h"
#include "ui/nodes/window/Window.h"
#include "ui/lector_aphelui/reader/AphluiReader.h"

void printTree(const std::shared_ptr<Node>& node, int depth = 0) {
    if (!node) return;

    std::string indent(depth * 4, ' ');

    auto winRef = std::dynamic_pointer_cast<WindowNode>(node);
    auto spaceRef = std::dynamic_pointer_cast<Space2D>(node);
    auto visualRef = std::dynamic_pointer_cast<VisualNode>(node);

    if (winRef) {
        std::cout << indent << "- " << winRef->getType() << " [Name: \"" << winRef->name 
                  << "\", Title: \"" << winRef->title << "\""
                  << ", Pos: (" << winRef->position.x << ", " << winRef->position.y << ")"
                  << ", Size: (" << winRef->size.width << ", " << winRef->size.height << ")"
                  // Muestra los 4 canales incluyendo la Transparencia/Alfa
                  << ", BgColor: RGBA(" << (int)winRef->backgroundColor.r << ", " 
                                        << (int)winRef->backgroundColor.g << ", " 
                                        << (int)winRef->backgroundColor.b << ", " 
                                        << (int)winRef->backgroundColor.a << ")]" << std::endl;
    }
    else if (spaceRef) {
        std::cout << indent << "- " << spaceRef->getType() << " [Name: \"" << spaceRef->name 
                  << "\", Pos: (" << spaceRef->position.x << ", " << spaceRef->position.y << ")"
                  << ", Size: (" << spaceRef->size.width << ", " << spaceRef->size.height << ")]" << std::endl;
    } 
    else if (visualRef) {
        std::cout << indent << "- " << visualRef->getType() << " [Name: \"" << visualRef->name 
                  << "\", Pos: (" << visualRef->position.x << ", " << visualRef->position.y << ")]" << std::endl;
    } 
    else {
        // Imprime dinámicamente el tipo genérico cargado
        std::cout << indent << "- " << node->getType() << " [Name: \"" << node->name << "\"]" << std::endl;
    }

    for (const auto& child : node->children) {
        printTree(child, depth + 1);
    }
}

int main(int argc, char* argv[]) {
    std::string filePath = "../src/ui/Aphel_Ui/example.aphlui";
    if (argc > 1) filePath = argv[1];

    std::cout << "=== APHEL ENGINE v0.0.0.8 ===" << std::endl;

    std::shared_ptr<Node> root = AphluiReader::loadFromFile(filePath);

    if (root) {
        std::cout << "--- Árbol de Nodos Generado en RAM ---" << std::endl;
        printTree(root);
        std::cout << "--------------------------------------" << std::endl;
    }

    return 0;
}