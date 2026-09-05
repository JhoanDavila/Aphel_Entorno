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

Token Tokenizer::makeNumber() {
    int startLine = line;
    int startCol = column;
    size_t startPos = index;

    // Procesa el signo negativo opcional
    if (peek() == '-') {
        advance();
    }

    bool isFloat = false;

    while (!isAtEnd()) {
        char c = peek();
        if (std::isdigit(c)) {
            advance();
        } else if (c == '.' && !isFloat) {
            // Verifica que el siguiente carácter sea un dígito para confirmar que es un flotante válido
            if (index + 1 < source.length() && std::isdigit(source[index + 1])) {
                isFloat = true;
                advance(); // Consume el punto
            } else {
                break;
            }
        } else {
            break;
        }
    }

    std::string value = source.substr(startPos, index - startPos);
    TokenType type = isFloat ? TokenType::FLOAT_LITERAL : TokenType::INT_LITERAL;

    return Token(type, std::move(value), startLine, startCol);
}

Token Tokenizer::makeIdentifierOrKeyword() {
    int startLine = line;
    int startCol = column;

    size_t startPos = index;
    while (!isAtEnd() && (std::isalnum(peek()) || peek() == '_')) {
        advance();
    }

    std::string value = source.substr(startPos, index - startPos);

    // Palabras clave de control
    if (value == "File") return Token(TokenType::KEYWORD_FILE, std::move(value), startLine, startCol);
    if (value == "Render") return Token(TokenType::KEYWORD_RENDER, std::move(value), startLine, startCol);

    // Keyword de declaración de variables
    if (value == "data") return Token(TokenType::KEYWORD_DATA, std::move(value), startLine, startCol);

    // Tipos de datos
    if (value == "txt") return Token(TokenType::TYPE_TXT, std::move(value), startLine, startCol);
    if (value == "int") return Token(TokenType::TYPE_INT, std::move(value), startLine, startCol);
    if (value == "dbl") return Token(TokenType::TYPE_DBL, std::move(value), startLine, startCol);
    if (value == "nat") return Token(TokenType::TYPE_NAT, std::move(value), startLine, startCol);
    if (value == "bool") return Token(TokenType::TYPE_BOOL, std::move(value), startLine, startCol);

    // Literales booleanos
    if (value == "true" || value == "false") {
        return Token(TokenType::BOOL_LITERAL, std::move(value), startLine, startCol);
    }

    // Identificadores (nombres de variables o alias de recursos)
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
        else if (c == ':') {
            advance();
            tokens.push_back(Token(TokenType::COLON, ":", currentLine, currentCol));
        }
        else if (c == ';') {
            advance();
            tokens.push_back(Token(TokenType::SEMICOLON, ";", currentLine, currentCol));
        }
        else if (c == '(') {
            advance();
            tokens.push_back(Token(TokenType::LPAREN, "(", currentLine, currentCol));
        }
        else if (c == ')') {
            advance();
            tokens.push_back(Token(TokenType::RPAREN, ")", currentLine, currentCol));
        }
        else if (std::isdigit(c) || (c == '-' && index + 1 < source.length() && std::isdigit(source[index + 1]))) {
            tokens.push_back(makeNumber());
        }
        else if (std::isalpha(c) || c == '_') {
            tokens.push_back(makeIdentifierOrKeyword());
        }
        else {
            std::string unknownChar(1, advance());
            tokens.push_back(Token(TokenType::UNKNOWN, std::move(unknownChar), currentLine, currentCol));
        }
    }

    tokens.push_back(Token(TokenType::END_OF_FILE, "", line, column));
    return tokens;
}

} // namespace AphelG