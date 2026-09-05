#ifndef APHELG_TOKEN_H
#define APHELG_TOKEN_H

#include <string>

namespace AphelG {

enum class TokenType {
    // Palabras Reservadas (Keywords de Control)
    KEYWORD_FILE,      // "File"
    KEYWORD_RENDER,    // "Render"

    // Declaración de Datos
    KEYWORD_DATA,      // "data"
    
    // Tipos de Datos
    TYPE_TXT,          // "txt"
    TYPE_INT,          // "int"
    TYPE_DBL,          // "dbl"
    TYPE_NAT,          // "nat"
    TYPE_BOOL,         // "bool"

    // Símbolos y Operadores
    ASSIGN,            // "="
    COLON,             // ":"
    SEMICOLON,         // ";"
    LPAREN,            // "("
    RPAREN,            // ")"

    // Literales e Identificadores
    IDENTIFIER,        // Nombre de variable o recurso (ej: usuario, ventana)
    STRING_LITERAL,    // Cadenas entre comillas (ej: "Jhoan", "--;ui;example.aphlui")
    INT_LITERAL,       // Números enteros o naturales (ej: 4, -5)
    FLOAT_LITERAL,     // Números con punto flotante (ej: 0.85, -3.14)
    BOOL_LITERAL,      // "true", "false", "1", "0"

    // Control y Diagnóstico
    END_OF_FILE,       // Fin de archivo
    UNKNOWN            // Token no reconocido
};

struct Token {
    TokenType type;
    std::string value;
    int line;
    int column;

    Token(TokenType t, const std::string& v, int l, int c)
        : type(t), value(v), line(l), column(c) {}
};

} // namespace AphelG

#endif // APHELG_TOKEN_H