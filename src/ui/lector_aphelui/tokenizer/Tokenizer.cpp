#include "ui/lector_aphelui/tokenizer/Tokenizer.h"
#include "ui/lector_aphelui/error_manager/ErrorManager.h"
#include <algorithm>
#include <unordered_set>

std::vector<Token> Tokenizer::tokenize(const std::vector<std::string>& cleanLines) {
    std::vector<Token> tokens;

    // Lista de nodos válidos que reconoce el motor
    static const std::unordered_set<std::string> validNodeTypes = {
        "Node", "VisualNode", "Space2D", "Window"
    };

    int lineNumber = 0;

    for (const auto& line : cleanLines) {
        lineNumber++; // Incrementamos contador de línea
        
        size_t firstNonSpace = line.find_first_not_of(" \t");
        if (firstNonSpace == std::string::npos) continue;

        int indent = static_cast<int>(firstNonSpace);
        std::string trimmedLine = line.substr(firstNonSpace);

        // 1. Caso Atributo (.position:, .size:, .title:, etc.)
        if (trimmedLine[0] == '.') {
            size_t colonPos = trimmedLine.find(':');
            
            // Error: Es un atributo pero le falta los dos puntos ':'
            if (colonPos == std::string::npos) {
                ErrorManager::logError(lineNumber, "Missing ':' in property declaration: '" + trimmedLine + "'");
                tokens.push_back({TokenType::UNKNOWN, trimmedLine, lineNumber});
                continue;
            }

            std::string propName = trimmedLine.substr(0, colonPos);
            tokens.push_back({TokenType::PROPERTY, propName, lineNumber});

            std::string rawVal = trimmedLine.substr(colonPos + 1);
            size_t valStart = rawVal.find_first_not_of(" \t");
            size_t valEnd = rawVal.find_last_not_of(" \t\r\n");

            if (valStart != std::string::npos) {
                std::string cleanVal = rawVal.substr(valStart, (valEnd - valStart + 1));

                // Limpiar comillas si las tiene
                if (cleanVal.front() == '"' && cleanVal.back() == '"' && cleanVal.length() >= 2) {
                    cleanVal = cleanVal.substr(1, cleanVal.length() - 2);
                }

                tokens.push_back({TokenType::VALUE, cleanVal, lineNumber});
            } else {
                // Error: Propiedad sin valor asignado (.title:)
                ErrorManager::logError(lineNumber, "Property '" + propName + "' has no value assigned.");
            }
        } 
        // 2. Caso Definición de Nodo (Window:, VisualNode:, etc.)
        else {
            size_t colonPos = trimmedLine.find(':');
            
            // Error: No empieza por '.' y no tiene ':', es texto suelto/basura
            if (colonPos == std::string::npos) {
                ErrorManager::logError(lineNumber, "Unrecognized syntax or missing ':' in statement: '" + trimmedLine + "'");
                tokens.push_back({TokenType::UNKNOWN, trimmedLine, lineNumber});
                continue;
            }

            std::string nodeType = trimmedLine.substr(0, colonPos);
            size_t typeEnd = nodeType.find_last_not_of(" \t");
            if (typeEnd != std::string::npos) {
                nodeType = nodeType.substr(0, typeEnd + 1);
            }

            // Error: El tipo de nodo no existe en el motor (ej. "BotonDesconocido:")
            if (validNodeTypes.find(nodeType) == validNodeTypes.end()) {
                ErrorManager::logError(lineNumber, "Unknown Node type: '" + nodeType + "'");
                tokens.push_back({TokenType::UNKNOWN, nodeType, lineNumber});
                continue;
            }

            tokens.push_back({TokenType::NODE_TYPE, nodeType, lineNumber});
        }
    }

    return tokens;
}