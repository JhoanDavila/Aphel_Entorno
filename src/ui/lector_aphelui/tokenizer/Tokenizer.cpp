#include "ui/lector_aphelui/tokenizer/Tokenizer.h"
#include "ui/lector_aphelui/error_manager/ErrorManager.h"
#include <algorithm>
#include <sstream>
#include <unordered_set>

std::vector<Token> Tokenizer::tokenize(const std::vector<std::string>& cleanLines) {
    std::vector<Token> tokens;

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

                // Se mantiene cleanVal intacto (con comillas incluidas) 
                // para que Parser::isValidStringFormat pueda validarlo adecuadamente.
                tokens.push_back({TokenType::VALUE, cleanVal, lineNumber, indent});
            } else {
                ErrorManager::logError(lineNumber, "Property '" + propName + "' has no value assigned.");
            }
        } 
        // 2. Caso Definición de Nodo
        else {
            size_t colonPos = trimmedLine.find(':');
            
            if (colonPos == std::string::npos) {
                ErrorManager::logError(lineNumber, "Unrecognized syntax or missing ':' in statement: '" + trimmedLine + "'");
                tokens.push_back({TokenType::UNKNOWN, trimmedLine, lineNumber, indent});
                continue;
            }

            std::string header = trimmedLine.substr(0, colonPos);
            std::stringstream ss(header);
            std::string nodeType, nodeName;

            ss >> nodeType; 
            ss >> nodeName; 

            if (validNodeTypes.find(nodeType) == validNodeTypes.end()) {
                ErrorManager::logError(lineNumber, "Unknown Node type: '" + nodeType + "'");
                tokens.push_back({TokenType::UNKNOWN, nodeType, lineNumber, indent});
                continue;
            }

            tokens.push_back({TokenType::NODE_TYPE, nodeType, lineNumber, indent});

            if (!nodeName.empty()) {
                tokens.push_back({TokenType::NODE_NAME, nodeName, lineNumber, indent});
            }
        }
    }

    return tokens;
}