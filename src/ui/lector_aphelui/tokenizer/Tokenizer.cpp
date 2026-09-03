#include "ui/lector_aphelui/tokenizer/Tokenizer.h"
#include <algorithm>

std::vector<Token> Tokenizer::tokenize(const std::vector<std::string>& cleanLines) {
    std::vector<Token> tokens;

    for (const auto& line : cleanLines) {
        size_t firstNonSpace = line.find_first_not_of(" \t");
        if (firstNonSpace == std::string::npos) continue;

        int indent = static_cast<int>(firstNonSpace);
        std::string trimmedLine = line.substr(firstNonSpace);

        // 1. Si la línea es un Atributo (.name, .position)
        if (trimmedLine[0] == '.') {
            size_t colonPos = trimmedLine.find(':');
            if (colonPos != std::string::npos) {
                std::string propName = trimmedLine.substr(0, colonPos);
                tokens.push_back({TokenType::PROPERTY, propName, indent});

                std::string rawVal = trimmedLine.substr(colonPos + 1);
                size_t valStart = rawVal.find_first_not_of(" \t");
                size_t valEnd = rawVal.find_last_not_of(" \t\r\n");

                if (valStart != std::string::npos) {
                    std::string cleanVal = rawVal.substr(valStart, (valEnd - valStart + 1));

                    // Limpiar comillas si las tiene
                    if (cleanVal.front() == '"' && cleanVal.back() == '"' && cleanVal.length() >= 2) {
                        cleanVal = cleanVal.substr(1, cleanVal.length() - 2);
                    }

                    tokens.push_back({TokenType::VALUE, cleanVal, indent});
                }
            }
        } 
        // 2. Si es definición de Nodo (Node: o VisualNode:)
        else {
            size_t colonPos = trimmedLine.find(':');
            std::string nodeType = (colonPos != std::string::npos) 
                                    ? trimmedLine.substr(0, colonPos) 
                                    : trimmedLine;
            
            size_t typeEnd = nodeType.find_last_not_of(" \t");
            if (typeEnd != std::string::npos) {
                nodeType = nodeType.substr(0, typeEnd + 1);
            }

            tokens.push_back({TokenType::NODE_TYPE, nodeType, indent});
        }
    }

    return tokens;
}