#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>

// Inclusiones respetando tu árbol de carpetas
#include "ui/nodes/node/Node.h"
#include "ui/lector_aphelui/cleaner/Cleaner.h"
#include "ui/lector_aphelui/tokenizer/Tokenizer.h"
#include "ui/lector_aphelui/parser/Parser.h"

void printTree(const std::shared_ptr<Node>& node, int depth = 0) {
    if (!node) return;

    std::string indent(depth * 4, ' ');
    std::cout << indent << "- Node [Name: \"" << node->name << "\"]" << std::endl;

    for (const auto& child : node->children) {
        printTree(child, depth + 1);
    }
}

int main(int argc, char* argv[]) {
    // Ruta por defecto apuntando a tu carpeta ui/Aphel_Ui/example.aphlui
    std::string filePath = "src/ui/Aphel_Ui/example.aphlui";

    if (argc > 1) {
        filePath = argv[1];
    }

    std::cout << "=== APHEL ENGINE v0.0.0.6 - INTEGRATION TEST ===" << std::endl;
    std::cout << "Leyendo archivo: " << filePath << std::endl << std::endl;

    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "[Error] No se pudo abrir el archivo: " << filePath << std::endl;
        return 1;
    }

    std::vector<std::string> rawLines;
    std::string line;
    while (std::getline(file, line)) {
        rawLines.push_back(line);
    }
    file.close();

    // Pipeline
    std::vector<std::string> cleanLines = Cleaner::removeCommentsAndEmptyLines(rawLines);
    std::vector<Token> tokens = Tokenizer::tokenize(cleanLines);
    std::shared_ptr<Node> root = Parser::parse(tokens);

    if (root) {
        std::cout << "--- Árbol de Nodos Generado en Memoria ---" << std::endl;
        printTree(root);
        std::cout << "------------------------------------------" << std::endl;
        std::cout << "¡Éxito! El flujo completo funciona correctamente." << std::endl;
    } else {
        std::cerr << "[Error] El Parser no pudo generar el árbol de nodos." << std::endl;
        return 1;
    }

    return 0;
}