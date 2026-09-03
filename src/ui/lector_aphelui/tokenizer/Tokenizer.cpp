#include "ui/lector_aphelui/tokenizer/Tokenizer.h"
#include "ui/lector_aphelui/error_manager/ErrorManager.h"
#include <algorithm>
#include <sstream>
#include <unordered_set>

std::vector<Token> Tokenizer::tokenize(const std::vector<std::string>& cleanLines) {
    std::vector<Token> tokens;

    // Lista de tipos de nodos reconocidos por AphelEngine
    static const std::unordered_set<std::string> validNodeTypes = {
        "Node", "VisualNode", "Space2D", "Window"
    };

    int lineNumber = 0;

    for (const auto& line : cleanLines) {
        lineNumber++;
        
        size_t firstNonSpace = line.find_first_not_of(" \t");
        if (firstNonSpace == std::string::npos) continue;

        int indent = static_cast<int>(firstNonSpace);
        std::string trimmedLine = line.substr(firstNonSpace);

        // 1. Caso Atributo (.position:, .size:, .title:, etc.)
        if (trimmedLine[0] == '.') {
            size_t colonPos = trimmedLine.find(':');
            
            // Error: Atributo sin ':'
            if (colonPos == std::string::npos) {
                ErrorManager::logError(lineNumber, "Missing ':' in property declaration: '" + trimmedLine + "'");
                tokens.push_back({TokenType::UNKNOWN, trimmedLine, lineNumber, indent});
                continue;
            }

            std::string propName = trimmedLine.substr(0, colonPos);
            tokens.push_back({TokenType::PROPERTY, propName, lineNumber, indent});

            std::string rawVal = trimmedLine.substr(colonPos + 1);
            size_t valStart = rawVal.find_first_not_of(" \t");
            size_t valEnd = rawVal.find_last_not_of(" \t\r\n");

            if (valStart != std::string::npos) {
                std::string cleanVal = rawVal.substr(valStart, (valEnd - valStart + 1));

                // Limpiar comillas si las tiene
                if (cleanVal.front() == '"' && cleanVal.back() == '"' && cleanVal.length() >= 2) {
                    cleanVal = cleanVal.substr(1, cleanVal.length() - 2);
                }

                tokens.push_back({TokenType::VALUE, cleanVal, lineNumber, indent});
            } else {
                ErrorManager::logError(lineNumber, "Property '" + propName + "' has no value assigned.");
            }
        } 
        // 2. Caso Definición de Nodo (Window:, VisualNode main_node:, etc.)
        else {
            size_t colonPos = trimmedLine.find(':');
            
            // Error: No empieza por '.' y no tiene ':', es sintaxis no válida
            if (colonPos == std::string::npos) {
                ErrorManager::logError(lineNumber, "Unrecognized syntax or missing ':' in statement: '" + trimmedLine + "'");
                tokens.push_back({TokenType::UNKNOWN, trimmedLine, lineNumber, indent});
                continue;
            }

            std::string header = trimmedLine.substr(0, colonPos);
            std::stringstream ss(header);
            std::string nodeType, nodeName;

            ss >> nodeType; // Extrae la primera palabra (Tipo)
            ss >> nodeName; // Extrae la segunda palabra opcional (Nombre)

            // Validación: ¿El tipo de nodo existe en AphelEngine?
            if (validNodeTypes.find(nodeType) == validNodeTypes.end()) {
                ErrorManager::logError(lineNumber, "Unknown Node type: '" + nodeType + "'");
                tokens.push_back({TokenType::UNKNOWN, nodeType, lineNumber, indent});
                continue;
            }

            // Emite token del Tipo de Nodo
            tokens.push_back({TokenType::NODE_TYPE, nodeType, lineNumber, indent});

            // Si se definió un nombre opcional (ej. "main_window"), emite token de Nombre
            if (!nodeName.empty()) {
                tokens.push_back({TokenType::NODE_NAME, nodeName, lineNumber, indent});
            }
        }
    }

    return tokens;
}