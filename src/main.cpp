#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>

#include "ui/nodes/node/Node.h"
#include "ui/nodes/visual_node/VisualNode.h"
#include "ui/lector_aphelui/cleaner/Cleaner.h"
#include "ui/lector_aphelui/tokenizer/Tokenizer.h"
#include "ui/lector_aphelui/parser/Parser.h"

void printTree(const std::shared_ptr<Node>& node, int depth = 0) {
    if (!node) return;

    std::string indent(depth * 4, ' ');
    
    // Si el nodo es un VisualNode, mostramos también su posición
    auto visualRef = std::dynamic_pointer_cast<VisualNode>(node);
    if (visualRef) {
        std::cout << indent << "- VisualNode [Name: \"" << visualRef->name 
                  << "\", Position: (" << visualRef->position.x << ", " << visualRef->position.y << ")]" << std::endl;
    } else {
        std::cout << indent << "- Node [Name: \"" << node->name << "\"]" << std::endl;
    }

    for (const auto& child : node->children) {
        printTree(child, depth + 1);
    }
}

int main(int argc, char* argv[]) {
    std::string filePath = "../src/ui/Aphel_Ui/example.aphlui";
    if (argc > 1) filePath = argv[1];

    std::cout << "=== APHEL ENGINE v0.0.0.7 ===" << std::endl;

    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "[Error] No se pudo abrir: " << filePath << std::endl;
        return 1;
    }

    std::vector<std::string> rawLines;
    std::string line;
    while (std::getline(file, line)) rawLines.push_back(line);
    file.close();

    std::vector<std::string> cleanLines = Cleaner::removeCommentsAndEmptyLines(rawLines);
    std::vector<Token> tokens = Tokenizer::tokenize(cleanLines);
    std::shared_ptr<Node> root = Parser::parse(tokens);

    if (root) {
        std::cout << "--- Árbol de Nodos Generado ---" << std::endl;
        printTree(root);
        std::cout << "-------------------------------" << std::endl;
    }

    return 0;
}