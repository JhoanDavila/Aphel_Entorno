#include "ui/lector_aphelui/parser/Parser.h"
#include "ui/lector_aphelui/error_manager/ErrorManager.h"
#include <stack>
#include <sstream>
#include <regex>
#include <algorithm>

// --- Métodos de Utilidad ---

static std::shared_ptr<Node> createNodeByType(const std::string& type) {
    if (type == "Window")     return std::make_shared<WindowNode>(type);
    if (type == "Space2D")    return std::make_shared<Space2D>(type);
    if (type == "VisualNode") return std::make_shared<VisualNode>(type);
    return std::make_shared<Node>(type);
}

static std::string stripQuotes(const std::string& str) {
    if (str.length() >= 2 && str.front() == '"' && str.back() == '"') {
        return str.substr(1, str.length() - 2);
    }
    return str;
}

bool Parser::isValidStringFormat(const std::string& valStr) {
    return valStr.length() >= 2 && valStr.front() == '"' && valStr.back() == '"';
}

bool Parser::tryParseVector2D(const std::string& valStr, Vector2D& outVec) {
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

bool Parser::tryParseColor(const std::string& valStr, Color& outColor, int line) {
    std::string clean = stripQuotes(valStr);

    // 1. Formato envoltorio: HEX(FF0000) o hex(FF0000)
    static const std::regex hexWrapperRegex(R"(^HEX\s*\(\s*([0-9a-fA-F]{3}|[0-9a-fA-F]{6}|[0-9a-fA-F]{8})\s*\)$)", std::regex::icase);
    std::smatch hexWrapMatch;
    if (std::regex_match(clean, hexWrapMatch, hexWrapperRegex)) {
        clean = hexWrapMatch[1].str();
    }

    // 2. Formato HEX directo (ejemplo: FF0000, F00, FF0000FF)
    static const std::regex hexRegex(R"(^([0-9a-fA-F]{3}|[0-9a-fA-F]{6}|[0-9a-fA-F]{8})$)");
    if (std::regex_match(clean, hexRegex)) {

        if (clean.length() == 3) {
            std::string expanded;
            for (char c : clean) { expanded += c; expanded += c; }
            clean = expanded;
        }

        try {
            unsigned long hexVal = std::stoul(clean, nullptr, 16);
            if (clean.length() == 6) {
                outColor.r = static_cast<unsigned char>((hexVal >> 16) & 0xFF);
                outColor.g = static_cast<unsigned char>((hexVal >> 8) & 0xFF);
                outColor.b = static_cast<unsigned char>(hexVal & 0xFF);
                outColor.a = 255;
            } else if (clean.length() == 8) {
                outColor.r = static_cast<unsigned char>((hexVal >> 24) & 0xFF);
                outColor.g = static_cast<unsigned char>((hexVal >> 16) & 0xFF);
                outColor.b = static_cast<unsigned char>((hexVal >> 8) & 0xFF);
                outColor.a = static_cast<unsigned char>(hexVal & 0xFF);
            }
            return true;
        } catch (...) {
            ErrorManager::logError(line, "Failed to parse HEX value: " + clean);
            return false;
        }
    }

    // 3. Formatos rgb(...), rgba(...) y tuplas (r, g, b, a)
    static const std::regex prefixRegex(R"(^(rgb|rgba)?\s*\((.*)\)$)", std::regex::icase);
    std::smatch prefixMatch;

    if (std::regex_match(clean, prefixMatch, prefixRegex)) {
        std::string prefix = prefixMatch[1].str();
        std::transform(prefix.begin(), prefix.end(), prefix.begin(), ::tolower);
        std::string innerArgs = prefixMatch[2].str();

        std::stringstream ss(innerArgs);
        std::string segment;
        std::vector<int> values;

        while (std::getline(ss, segment, ',')) {
            segment.erase(0, segment.find_first_not_of(" \t\r\n"));
            segment.erase(segment.find_last_not_of(" \t\r\n") + 1);

            try {
                if (segment.empty()) throw std::invalid_argument("Empty segment");
                size_t parsedChars = 0;
                int val = std::stoi(segment, &parsedChars);
                if (parsedChars != segment.length()) throw std::invalid_argument("Trailing characters");
                values.push_back(val);
            } catch (...) {
                ErrorManager::logError(line, "Invalid numeric value '" + segment + "' inside color declaration.");
                return false;
            }
        }

        if (prefix == "rgb" && values.size() != 3) {
            ErrorManager::logError(line, "'rgb()' format requires exactly 3 parameters (r, g, b). Got " + std::to_string(values.size()) + ".");
            return false;
        }
        if (prefix == "rgba" && values.size() != 4) {
            ErrorManager::logError(line, "'rgba()' format requires exactly 4 parameters (r, g, b, a). Got " + std::to_string(values.size()) + ".");
            return false;
        }
        if (prefix.empty() && values.size() != 3 && values.size() != 4) {
            ErrorManager::logError(line, "Unlabelled color tuple expects 3 or 4 parameters. Got " + std::to_string(values.size()) + ".");
            return false;
        }

        for (int v : values) {
            if (v < 0 || v > 255) {
                ErrorManager::logError(line, "Color component out of range (" + std::to_string(v) + "). Allowed values: [0, 255].");
                return false;
            }
        }

        outColor.r = static_cast<unsigned char>(values[0]);
        outColor.g = static_cast<unsigned char>(values[1]);
        outColor.b = static_cast<unsigned char>(values[2]);
        outColor.a = (values.size() == 4) ? static_cast<unsigned char>(values[3]) : 255;

        return true;
    }

    ErrorManager::logError(line, "Invalid color specification: '" + clean + "'. Expected HEX(code), raw HEX, rgb(), rgba(), or tuple.");
    return false;
}

static void applyPropertyToNode(const std::shared_ptr<Node>& node, const Token& propToken, const Token& valToken) {
    const std::string& propName = propToken.value;
    const std::string& propValue = valToken.value;
    int line = valToken.line;

    if (propName == ".name") {
        if (Parser::isValidStringFormat(propValue)) {
            node->name = stripQuotes(propValue);
        } else {
            ErrorManager::logError(line, "Type Mismatch: '.name' expects double-quoted string.");
        }
    } 
    else if (propName == ".position") {
        if (auto visualRef = std::dynamic_pointer_cast<VisualNode>(node)) {
            Vector2D vec;
            if (Parser::tryParseVector2D(propValue, vec)) {
                visualRef->position = vec;
            } else {
                ErrorManager::logError(line, "Type Mismatch: '.position' expects tuple (x, y).");
            }
        } else {
            ErrorManager::logError(propToken.line, "Property '.position' is not supported on node type '" + node->getType() + "'.");
        }
    }
    else if (propName == ".size") {
        if (auto spaceRef = std::dynamic_pointer_cast<Space2D>(node)) {
            Vector2D vec;
            if (Parser::tryParseVector2D(propValue, vec)) {
                spaceRef->size = {vec.x, vec.y};
            } else {
                ErrorManager::logError(line, "Type Mismatch: '.size' expects tuple (width, height).");
            }
        } else {
            ErrorManager::logError(propToken.line, "Property '.size' is not supported on node type '" + node->getType() + "'.");
        }
    }
    else if (propName == ".title") {
        if (auto winRef = std::dynamic_pointer_cast<WindowNode>(node)) {
            if (Parser::isValidStringFormat(propValue)) {
                winRef->title = stripQuotes(propValue);
            } else {
                ErrorManager::logError(line, "Type Mismatch: '.title' expects double-quoted string.");
            }
        } else {
            ErrorManager::logError(propToken.line, "Property '.title' is not supported on node type '" + node->getType() + "'.");
        }
    }
    else if (propName == ".backgroundcolor") {
        if (auto winRef = std::dynamic_pointer_cast<WindowNode>(node)) {
            Color color;
            if (Parser::tryParseColor(propValue, color, line)) {
                winRef->backgroundColor = color;
            }
        } else {
            ErrorManager::logError(propToken.line, "Property '.backgroundcolor' is not supported on node type '" + node->getType() + "'.");
        }
    }
    else {
        ErrorManager::logError(propToken.line, "Unknown property '" + propName + "' on node type '" + node->getType() + "'.");
    }
}

// --- Método Principal del Parsing ---

std::shared_ptr<Node> Parser::parse(const std::vector<Token>& tokens) {
    if (tokens.empty()) return nullptr;

    // Validar raíz
    auto firstNodeIt = std::find_if(tokens.begin(), tokens.end(), [](const Token& t) {
        return t.type == TokenType::NODE_TYPE;
    });

    if (firstNodeIt == tokens.end() || firstNodeIt->value != "Window") {
        int errLine = (firstNodeIt != tokens.end()) ? firstNodeIt->line : 1;
        std::string foundType = (firstNodeIt != tokens.end()) ? firstNodeIt->value : "None";
        ErrorManager::logError(errLine, "Root element error: Expected root node 'Window', found '" + foundType + "'.");
        return nullptr;
    }

    std::shared_ptr<Node> root = nullptr;
    std::stack<std::pair<int, std::shared_ptr<Node>>> nodeStack;

    for (size_t i = 0; i < tokens.size(); ++i) {
        const auto& token = tokens[i];

        if (token.type == TokenType::NODE_TYPE) {
            auto currentNode = createNodeByType(token.value);

            // Asignar identificador opcional si el siguiente token es de tipo NODE_NAME
            if (i + 1 < tokens.size() && tokens[i + 1].type == TokenType::NODE_NAME) {
                currentNode->name = tokens[i + 1].value;
                i++;
            }

            // Consumir bloque de propiedades asociadas
            size_t peek = i + 1;
            while (peek < tokens.size() && tokens[peek].type == TokenType::PROPERTY) {
                const auto& propToken = tokens[peek];

                if (propToken.value.empty() || propToken.value.front() != '.') {
                    ErrorManager::logError(propToken.line, "Property name must begin with '.' (got '" + propToken.value + "').");
                    peek++;
                    if (peek < tokens.size() && tokens[peek].type == TokenType::VALUE) peek++;
                    continue;
                }

                if (peek + 1 < tokens.size() && tokens[peek + 1].type == TokenType::VALUE) {
                    applyPropertyToNode(currentNode, propToken, tokens[peek + 1]);
                    peek += 2;
                } else {
                    ErrorManager::logError(propToken.line, "Property '" + propToken.value + "' missing assignment value.");
                    peek++;
                }
            }

            i = peek - 1;

            if (!root) {
                root = currentNode;
                nodeStack.push({token.indentLevel, currentNode});
                continue;
            }

            // Gestionar jerarquía por identación con la pila
            while (!nodeStack.empty() && nodeStack.top().first >= token.indentLevel) {
                nodeStack.pop();
            }

            if (!nodeStack.empty()) {
                int parentIndent = nodeStack.top().first;
                if (token.indentLevel <= parentIndent) {
                    ErrorManager::logError(
                        token.line,
                        "Indentation error: Node '" + token.value + "' requires indentation level > " +
                        std::to_string(parentIndent) + " (found " + std::to_string(token.indentLevel) + ")."
                    );
                }
                nodeStack.top().second->addChild(currentNode);
            }

            nodeStack.push({token.indentLevel, currentNode});
        }
    }

    return root;
}