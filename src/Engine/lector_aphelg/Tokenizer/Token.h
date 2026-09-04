#ifndef APHELG_TOKEN_H
#define APHELG_TOKEN_H

#include <string>

namespace AphelG {

enum class TokenType {
    // Palabras Reservadas (Keywords)
    KEYWORD_FILE,      // "File"
    KEYWORD_RENDER,    // "Render"

    // Símbolos y Operadores
    ASSIGN,            // "="
    LPAREN,            // "("
    RPAREN,            // ")"

    // Literales e Identificadores
    IDENTIFIER,        // Nombre del recurso (ej: ventana)
    STRING_LITERAL,    // Ruta o cadena delimitada por comillas (ej: "--;--;ui;Aphel_Ui;example.aphlui")

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