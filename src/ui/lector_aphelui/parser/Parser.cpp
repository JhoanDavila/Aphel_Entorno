#include "ui/lector_aphelui/parser/Parser.h"
#include "ui/lector_aphelui/error_manager/ErrorManager.h"
#include <stack>
#include <sstream>

Vector2D Parser::parseVector2D(const std::string& valStr) {
    Vector2D vec{0, 0};
    std::string clean = valStr;
    if (!clean.empty() && clean.front() == '(') clean.erase(0, 1);
    if (!clean.empty() && clean.back() == ')') clean.pop_back();

    size_t commaPos = clean.find(',');
    if (commaPos != std::string::npos) {
        try {
            vec.x = std::stoi(clean.substr(0, commaPos));
            vec.y = std::stoi(clean.substr(commaPos + 1));
        } catch (...) {
            vec.x = 0; vec.y = 0;
        }
    }
    return vec;
}

Color Parser::parseColor(const std::string& valStr) {
    Color color{0, 0, 0, 255};
    std::string clean = valStr;

    // 1. Formato Hexadecimal (#RRGGBB o #RRGGBBAA o #RGB)
    if (!clean.empty() && clean.front() == '#') {
        clean.erase(0, 1);
        
        if (clean.length() == 3) {
            std::string expanded;
            for (char c : clean) { expanded += c; expanded += c; }
            clean = expanded;
        }

        try {
            unsigned int hexVal = std::stoul(clean, nullptr, 16);
            if (clean.length() == 6) {
                color.r = (hexVal >> 16) & 0xFF;
                color.g = (hexVal >> 8) & 0xFF;
                color.b = hexVal & 0xFF;
                color.a = 255;
            } else if (clean.length() == 8) {
                color.r = (hexVal >> 24) & 0xFF;
                color.g = (hexVal >> 16) & 0xFF;
                color.b = (hexVal >> 8) & 0xFF;
                color.a = hexVal & 0xFF;
            }
        } catch (...) {}
        return color;
    }

    // 2. Formato RGB(r,g,b) o RGBA(r,g,b,a)
    size_t start = clean.find('(');
    size_t end = clean.find(')');
    if (start != std::string::npos && end != std::string::npos && end > start) {
        clean = clean.substr(start + 1, end - start - 1);
    }

    std::stringstream ss(clean);
    std::string item;
    std::vector<int> values;
    while (std::getline(ss, item, ',')) {
        try { values.push_back(std::stoi(item)); } catch (...) {}
    }

    if (values.size() >= 3) {
        color.r = static_cast<unsigned char>(values[0]);
        color.g = static_cast<unsigned char>(values[1]);
        color.b = static_cast<unsigned char>(values[2]);
        color.a = (values.size() >= 4) ? static_cast<unsigned char>(values[3]) : 255;
    }

    return color;
}

std::shared_ptr<Node> Parser::parse(const std::vector<Token>& tokens) {
    if (tokens.empty()) return nullptr;

    std::shared_ptr<Node> root = nullptr;
    std::stack<std::pair<int, std::shared_ptr<Node>>> nodeStack;

    for (size_t i = 0; i < tokens.size(); ++i) {
        const auto& token = tokens[i];

        if (token.type == TokenType::NODE_TYPE) {
            std::shared_ptr<Node> currentNode = nullptr;

            // Instanciación polimórfica según la jerarquía
            if (token.value == "Window") {
                currentNode = std::make_shared<WindowNode>(token.value);
            } else if (token.value == "Space2D") {
                currentNode = std::make_shared<Space2D>(token.value);
            } else if (token.value == "VisualNode") {
                currentNode = std::make_shared<VisualNode>(token.value);
            } else {
                currentNode = std::make_shared<Node>(token.value);
            }

            // Si el siguiente token es un NODE_NAME (ej: "main_window")
            if (i + 1 < tokens.size() && tokens[i + 1].type == TokenType::NODE_NAME) {
                currentNode->name = tokens[i + 1].value;
                i++; // Avanzamos el índice para saltar el NODE_NAME
            }

            // Procesar propiedades
            size_t peek = i + 1;
            while (peek < tokens.size() && tokens[peek].type == TokenType::PROPERTY) {
                const auto& propToken = tokens[peek];
                std::string propName = propToken.value;

                // Validación 1: Verificar que empiece obligatoriamente por punto '.'
                if (propName.empty() || propName.front() != '.') {
                    ErrorManager::logError(propToken.line, "Property declaration must start with '.' (got '" + propName + "')");
                    peek++;
                    if (peek < tokens.size() && tokens[peek].type == TokenType::VALUE) peek++;
                    continue;
                }

                if (peek + 1 < tokens.size() && tokens[peek + 1].type == TokenType::VALUE) {
                    std::string propValue = tokens[peek + 1].value;

                    // --- Propiedades de la clase base Node ---
                    if (propName == ".name") {
                        currentNode->name = propValue;
                    } 
                    // --- Propiedades de VisualNode (y derivados como Space2D, WindowNode) ---
                    else if (propName == ".position") {
                        auto visualRef = std::dynamic_pointer_cast<VisualNode>(currentNode);
                        if (visualRef) {
                            visualRef->position = parseVector2D(propValue);
                        } else {
                            ErrorManager::logError(propToken.line, "Property '.position' is not supported by node type '" + token.value + "'");
                        }
                    }
                    // --- Propiedades de Space2D (y derivados como WindowNode) ---
                    else if (propName == ".size") {
                        auto spaceRef = std::dynamic_pointer_cast<Space2D>(currentNode);
                        if (spaceRef) {
                            Vector2D vec = parseVector2D(propValue);
                            spaceRef->size = {vec.x, vec.y};
                        } else {
                            ErrorManager::logError(propToken.line, "Property '.size' is not supported by node type '" + token.value + "'");
                        }
                    }
                    // --- Propiedades exclusivas de WindowNode ---
                    else if (propName == ".title") {
                        auto winRef = std::dynamic_pointer_cast<WindowNode>(currentNode);
                        if (winRef) {
                            winRef->title = propValue;
                        } else {
                            ErrorManager::logError(propToken.line, "Property '.title' is not supported by node type '" + token.value + "'");
                        }
                    }
                    else if (propName == ".backgroundcolor") {
                        auto winRef = std::dynamic_pointer_cast<WindowNode>(currentNode);
                        if (winRef) {
                            winRef->backgroundColor = parseColor(propValue);
                        } else {
                            ErrorManager::logError(propToken.line, "Property '.backgroundcolor' is not supported by node type '" + token.value + "'");
                        }
                    }
                    // --- Caso: Nombre de propiedad no reconocido ---
                    else {
                        ErrorManager::logError(propToken.line, "Unknown property '" + propName + "' for node type '" + token.value + "'");
                    }

                    peek += 2; // Avanzar propiedad y valor
                } else {
                    ErrorManager::logError(propToken.line, "Property '" + propName + "' missing value.");
                    peek++;
                }
            }

            i = peek - 1;

            // Construcción del Árbol de Jerarquía
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