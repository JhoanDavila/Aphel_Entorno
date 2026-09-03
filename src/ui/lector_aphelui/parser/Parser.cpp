#include "ui/lector_aphelui/parser/Parser.h"
#include "ui/lector_aphelui/error_manager/ErrorManager.h"
#include <stack>
#include <sstream>
#include <regex>

// Parsea (x, y) asegurando que ambos sean números enteros
bool Parser::tryParseVector2D(const std::string& valStr, Vector2D& outVec) {
    // Regex que valida exactamente el formato (entero, entero) permitiendo espacios opcionales
    static const std::regex vecRegex(R"(\(\s*(-?\d+)\s*,\s*(-?\d+)\s*\))");
    std::smatch match;

    if (std::regex_match(valStr, match, vecRegex)) {
        try {
            outVec.x = std::stoi(match[1].str());
            outVec.y = std::stoi(match[2].str());
            return true;
        } catch (...) {
            return false;
        }
    }
    return false;
}

// Parsea formato Hex (#RGB, #RRGGBB, #RRGGBBAA) o RGB/RGBA(r, g, b, a)
bool Parser::tryParseColor(const std::string& valStr, Color& outColor) {
    std::string clean = valStr;

    // 1. Formato Hexadecimal
    if (!clean.empty() && clean.front() == '#') {
        static const std::regex hexRegex(R"(^#([0-9a-fA-F]{3}|[0-9a-fA-F]{6}|[0-9a-fA-F]{8})$)");
        if (!std::regex_match(clean, hexRegex)) return false;

        clean.erase(0, 1);
        if (clean.length() == 3) {
            std::string expanded;
            for (char c : clean) { expanded += c; expanded += c; }
            clean = expanded;
        }

        try {
            unsigned int hexVal = std::stoul(clean, nullptr, 16);
            if (clean.length() == 6) {
                outColor.r = (hexVal >> 16) & 0xFF;
                outColor.g = (hexVal >> 8) & 0xFF;
                outColor.b = hexVal & 0xFF;
                outColor.a = 255;
            } else if (clean.length() == 8) {
                outColor.r = (hexVal >> 24) & 0xFF;
                outColor.g = (hexVal >> 16) & 0xFF;
                outColor.b = (hexVal >> 8) & 0xFF;
                outColor.a = hexVal & 0xFF;
            }
            return true;
        } catch (...) {
            return false;
        }
    }

    // 2. Formato RGB / RGBA: (r, g, b) o (r, g, b, a)
    static const std::regex rgbRegex(R"(\(\s*(\d{1,3})\s*,\s*(\d{1,3})\s*,\s*(\d{1,3})\s*(?:,\s*(\d{1,3})\s*)?\))");
    std::smatch match;

    if (std::regex_match(clean, match, rgbRegex)) {
        try {
            int r = std::stoi(match[1].str());
            int g = std::stoi(match[2].str());
            int b = std::stoi(match[3].str());
            int a = match[4].matched ? std::stoi(match[4].str()) : 255;

            if (r > 255 || g > 255 || b > 255 || a > 255) return false;

            outColor.r = static_cast<unsigned char>(r);
            outColor.g = static_cast<unsigned char>(g);
            outColor.b = static_cast<unsigned char>(b);
            outColor.a = static_cast<unsigned char>(a);
            return true;
        } catch (...) {
            return false;
        }
    }

    return false;
}

// Comprueba que un valor tipo String esté entre comillas dobles
bool Parser::isValidStringFormat(const std::string& valStr) {
    return valStr.length() >= 2 && valStr.front() == '"' && valStr.back() == '"';
}

std::shared_ptr<Node> Parser::parse(const std::vector<Token>& tokens) {
    if (tokens.empty()) return nullptr;

    std::shared_ptr<Node> root = nullptr;
    std::stack<std::pair<int, std::shared_ptr<Node>>> nodeStack;

    for (size_t i = 0; i < tokens.size(); ++i) {
        const auto& token = tokens[i];

        if (token.type == TokenType::NODE_TYPE) {
            std::shared_ptr<Node> currentNode = nullptr;

            if (token.value == "Window") {
                currentNode = std::make_shared<WindowNode>(token.value);
            } else if (token.value == "Space2D") {
                currentNode = std::make_shared<Space2D>(token.value);
            } else if (token.value == "VisualNode") {
                currentNode = std::make_shared<VisualNode>(token.value);
            } else {
                currentNode = std::make_shared<Node>(token.value);
            }

            if (i + 1 < tokens.size() && tokens[i + 1].type == TokenType::NODE_NAME) {
                currentNode->name = tokens[i + 1].value;
                i++;
            }

            // Procesar propiedades
            size_t peek = i + 1;
            while (peek < tokens.size() && tokens[peek].type == TokenType::PROPERTY) {
                const auto& propToken = tokens[peek];
                std::string propName = propToken.value;

                if (propName.empty() || propName.front() != '.') {
                    ErrorManager::logError(propToken.line, "Property declaration must start with '.' (got '" + propName + "')");
                    peek++;
                    if (peek < tokens.size() && tokens[peek].type == TokenType::VALUE) peek++;
                    continue;
                }

                if (peek + 1 < tokens.size() && tokens[peek + 1].type == TokenType::VALUE) {
                    const auto& valToken = tokens[peek + 1];
                    std::string propValue = valToken.value;

                    // --- Propiedad .name (String) ---
                    if (propName == ".name") {
                        if (isValidStringFormat(propValue)) {
                            currentNode->name = propValue.substr(1, propValue.length() - 2);
                        } else {
                            ErrorManager::logError(valToken.line, "Type Mismatch: '.name' expects a string quoted with double quotes (e.g. \"my_name\")");
                        }
                    } 
                    // --- Propiedad .position (Vector2D) ---
                    else if (propName == ".position") {
                        auto visualRef = std::dynamic_pointer_cast<VisualNode>(currentNode);
                        if (visualRef) {
                            Vector2D vec;
                            if (tryParseVector2D(propValue, vec)) {
                                visualRef->position = vec;
                            } else {
                                ErrorManager::logError(valToken.line, "Type Mismatch: '.position' expects a Vector2D formatted as (x, y)");
                            }
                        } else {
                            ErrorManager::logError(propToken.line, "Property '.position' is not supported by node type '" + token.value + "'");
                        }
                    }
                    // --- Propiedad .size (Vector2D) ---
                    else if (propName == ".size") {
                        auto spaceRef = std::dynamic_pointer_cast<Space2D>(currentNode);
                        if (spaceRef) {
                            Vector2D vec;
                            if (tryParseVector2D(propValue, vec)) {
                                spaceRef->size = {vec.x, vec.y};
                            } else {
                                ErrorManager::logError(valToken.line, "Type Mismatch: '.size' expects formatted dimensions as (width, height)");
                            }
                        } else {
                            ErrorManager::logError(propToken.line, "Property '.size' is not supported by node type '" + token.value + "'");
                        }
                    }
                    // --- Propiedad .title (String) ---
                    else if (propName == ".title") {
                        auto winRef = std::dynamic_pointer_cast<WindowNode>(currentNode);
                        if (winRef) {
                            if (isValidStringFormat(propValue)) {
                                winRef->title = propValue.substr(1, propValue.length() - 2);
                            } else {
                                ErrorManager::logError(valToken.line, "Type Mismatch: '.title' expects a string quoted with double quotes (e.g. \"My Title\")");
                            }
                        } else {
                            ErrorManager::logError(propToken.line, "Property '.title' is not supported by node type '" + token.value + "'");
                        }
                    }
                    // --- Propiedad .backgroundcolor (Color) ---
                    else if (propName == ".backgroundcolor") {
                        auto winRef = std::dynamic_pointer_cast<WindowNode>(currentNode);
                        if (winRef) {
                            Color color;
                            if (tryParseColor(propValue, color)) {
                                winRef->backgroundColor = color;
                            } else {
                                ErrorManager::logError(valToken.line, "Type Mismatch: '.backgroundcolor' expects a Color in #HEX or (r, g, b, a) format");
                            }
                        } else {
                            ErrorManager::logError(propToken.line, "Property '.backgroundcolor' is not supported by node type '" + token.value + "'");
                        }
                    }
                    else {
                        ErrorManager::logError(propToken.line, "Unknown property '" + propName + "' for node type '" + token.value + "'");
                    }

                    peek += 2;
                } else {
                    ErrorManager::logError(propToken.line, "Property '" + propName + "' missing value.");
                    peek++;
                }
            }

            i = peek - 1;

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