#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType {
    NODE_TYPE,  // Para tipos de nodo (ej. "Node", "Button")
    PROPERTY,   // Para atributos que inician con punto (ej. ".name")
    VALUE,      // Para el valor entre comillas o texto (ej. "nodo_1")
    UNKNOWN
};

struct Token {
    TokenType type;
    std::string value;
    int indentLevel; // Guardamos los espacios al inicio para la v0.0.0.5
};

#endif // TOKEN_H