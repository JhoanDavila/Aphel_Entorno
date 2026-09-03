#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType {
    NODE_TYPE,    // p. ej. "Window", "VisualNode"
    NODE_NAME,    // p. ej. "main_window"
    PROPERTY,     // p. ej. ".position"
    VALUE,        // p. ej. "(100, 200)"
    UNKNOWN       // Basura o sintaxis no reconocida
};

struct Token {
    TokenType type;
    std::string value;
    int line;         // Número de línea para el ErrorManager
    int indentLevel;  // Espacios de sangría para la jerarquía del Parser
};

#endif // TOKEN_H