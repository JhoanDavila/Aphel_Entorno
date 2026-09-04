#include "Tokenizer.h"
#include <cctype>
#include <utility>

namespace AphelG {

Tokenizer::Tokenizer(const std::string& sourceCode)
    : source(sourceCode), index(0), line(1), column(1) {}

char Tokenizer::peek() const {
    if (isAtEnd()) return '\0';
    return source[index];
}

char Tokenizer::advance() {
    if (isAtEnd()) return '\0';
    char current = source[index++];
    if (current == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    return current;
}

bool Tokenizer::isAtEnd() const {
    return index >= source.length();
}

void Tokenizer::skipWhitespace() {
    while (!isAtEnd()) {
        char c = peek();
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            advance();
        } else {
            break;
        }
    }
}

Token Tokenizer::makeString() {
    int startLine = line;
    int startCol = column;
    
    advance(); // Consume la comilla inicial '"'
    
    size_t startPos = index;
    while (!isAtEnd() && peek() != '"') {
        advance();
    }

    std::string value = source.substr(startPos, index - startPos);

    if (isAtEnd()) {
        // La comilla no se cerró; devolvemos UNKNOWN para que el Parser lo gestione
        return Token(TokenType::UNKNOWN, std::move(value), startLine, startCol);
    }

    advance(); // Consume la comilla final '"'
    return Token(TokenType::STRING_LITERAL, std::move(value), startLine, startCol);
}

Token Tokenizer::makeIdentifierOrKeyword() {
    int startLine = line;
    int startCol = column;

    size_t startPos = index;
    while (!isAtEnd() && (std::isalnum(peek()) || peek() == '_')) {
        advance();
    }

    std::string value = source.substr(startPos, index - startPos);

    // Palabras clave reservadas
    if (value == "File") {
        return Token(TokenType::KEYWORD_FILE, std::move(value), startLine, startCol);
    }
    if (value == "Render") {
        return Token(TokenType::KEYWORD_RENDER, std::move(value), startLine, startCol);
    }

    // Identificadores válidos (ej: variables, nombres o texto suelto)
    return Token(TokenType::IDENTIFIER, std::move(value), startLine, startCol);
}

std::vector<Token> Tokenizer::tokenize() {
    std::vector<Token> tokens;
    tokens.reserve(source.length() / 4 + 8);

    while (!isAtEnd()) {
        skipWhitespace();
        if (isAtEnd()) break;

        char c = peek();
        int currentLine = line;
        int currentCol = column;

        if (c == '"') {
            tokens.push_back(makeString());
        }
        else if (c == '=') {
            advance();
            tokens.push_back(Token(TokenType::ASSIGN, "=", currentLine, currentCol));
        }
        else if (c == '(') {
            advance();
            tokens.push_back(Token(TokenType::LPAREN, "(", currentLine, currentCol));
        }
        else if (c == ')') {
            advance();
            tokens.push_back(Token(TokenType::RPAREN, ")", currentLine, currentCol));
        }
        else if (std::isalpha(c) || c == '_') {
            tokens.push_back(makeIdentifierOrKeyword());
        }
        else {
            std::string unknownChar(1, advance());
            // Carácter no reconocido se etiqueta como UNKNOWN
            tokens.push_back(Token(TokenType::UNKNOWN, std::move(unknownChar), currentLine, currentCol));
        }
    }

    tokens.push_back(Token(TokenType::END_OF_FILE, "", line, column));
    return tokens;
}

} // namespace AphelG