#include "ui/lector_aphelui/parser/Parser.h"
#include <stack>
#include <sstream>

Vector2D Parser::parseVector2D(const std::string& valStr) {
    Vector2D vec{0, 0};
    // Eliminamos los paréntesis si están presentes
    std::string clean = valStr;
    if (!clean.empty() && clean.front() == '(') clean.erase(0, 1);
    if (!clean.empty() && clean.back() == ')') clean.pop_back();

    size_t commaPos = clean.find(',');
    if (commaPos != std::string::npos) {
        try {
            vec.x = std::stoi(clean.substr(0, commaPos));
            vec.y = std::stoi(clean.substr(commaPos + 1));
        } catch (...) {
            vec.x = 0;
            vec.y = 0;
        }
    }
    return vec;
}

std::shared_ptr<Node> Parser::parse(const std::vector<Token>& tokens) {
    if (tokens.empty()) return nullptr;

    std::shared_ptr<Node> root = nullptr;
    std::stack<std::pair<int, std::shared_ptr<Node>>> nodeStack;

    for (size_t i = 0; i < tokens.size(); ++i) {
        const auto& token = tokens[i];

        if (token.type == TokenType::NODE_TYPE) {
            std::shared_ptr<Node> currentNode = nullptr;

            // 1. Instanciación Polimórfica según el tipo de nodo
            if (token.value == "VisualNode") {
                currentNode = std::make_shared<VisualNode>(token.value);
            } else {
                currentNode = std::make_shared<Node>(token.value);
            }

            // 2. Procesar dinámicamente todas las propiedades asociadas a este nodo
            size_t peek = i + 1;
            while (peek < tokens.size() && tokens[peek].type == TokenType::PROPERTY) {
                std::string propName = tokens[peek].value;
                
                // Si la propiedad tiene un valor correspondiente
                if (peek + 1 < tokens.size() && tokens[peek + 1].type == TokenType::VALUE) {
                    std::string propValue = tokens[peek + 1].value;

                    // Asignación de .name
                    if (propName == ".name") {
                        currentNode->name = propValue;
                    } 
                    // Asignación de .position (solo si es un VisualNode)
                    else if (propName == ".position") {
                        auto visualRef = std::dynamic_pointer_cast<VisualNode>(currentNode);
                        if (visualRef) {
                            visualRef->position = parseVector2D(propValue);
                        }
                    }

                    peek += 2; // Avanzamos la propiedad y su valor
                } else {
                    peek++;
                }
            }

            // Actualizamos el índice principal para no volver a iterar sobre las propiedades procesadas
            i = peek - 1;

            // 3. Ubicación del nodo dentro del árbol jerárquico
            if (!root) {
                root = currentNode;
                nodeStack.push({token.indentLevel, currentNode});
                continue;
            }

            while (!nodeStack.empty() && nodeStack.top().first >= token.indentLevel) {
                nodeStack.pop();
            }

            if (!nodeStack.empty()) {
                nodeStack.top().second->addChild(currentNode);
            }

            nodeStack.push({token.indentLevel, currentNode});
        }
    }

    return root;
}