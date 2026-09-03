#include "Tokenizer.h"
#include <algorithm>

std::vector<Token> Tokenizer::tokenize(const std::vector<std::string>& cleanLines) {
    std::vector<Token> tokens;

    for (const auto& line : cleanLines) {
        // 1. Contar la sangría (espacios al inicio)
        size_t firstNonSpace = line.find_first_not_of(" \t");
        if (firstNonSpace == std::string::npos) continue;

        int indent = static_cast<int>(firstNonSpace);
        std::string trimmedLine = line.substr(firstNonSpace);

        // 2. Si la línea es un Atributo (ej. .name: "nodo_1")
        if (trimmedLine[0] == '.') {
            size_t colonPos = trimmedLine.find(':');
            if (colonPos != std::string::npos) {
                // Token para la propiedad (ej. ".name")
                std::string propName = trimmedLine.substr(0, colonPos);
                tokens.push_back({TokenType::PROPERTY, propName, indent});

                // Token para el valor extraído entre comillas
                size_t startQuote = trimmedLine.find('"', colonPos);
                size_t endQuote = trimmedLine.rfind('"');
                if (startQuote != std::string::npos && endQuote > startQuote) {
                    std::string val = trimmedLine.substr(startQuote + 1, endQuote - startQuote - 1);
                    tokens.push_back({TokenType::VALUE, val, indent});
                }
            }
        } 
        // 3. Si la línea define un Nodo (ej. Node:)
        else {
            size_t colonPos = trimmedLine.find(':');
            std::string nodeType = (colonPos != std::string::npos) 
                                    ? trimmedLine.substr(0, colonPos) 
                                    : trimmedLine;
            tokens.push_back({TokenType::NODE_TYPE, nodeType, indent});
        }
    }

    return tokens;
}