#include "Parser.h"
#include "../ErrorManager/ErrorManager.h"
#include <stdexcept>

namespace AphelG {

Parser::Parser(std::vector<Token> tokenList)
    : tokens(std::move(tokenList)), current(0) {}

const Token& Parser::peek() const {
    return tokens[current];
}

const Token& Parser::previous() const {
    return tokens[current - 1];
}

bool Parser::isAtEnd() const {
    return peek().type == TokenType::END_OF_FILE;
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) return false;
    return peek().type == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

Token Parser::consume(TokenType type, const std::string& errorMessage) {
    if (check(type)) return advance();
    ErrorManager::logError(peek().line, errorMessage);
    return peek();
}

DataType Parser::parseDataType(TokenType type) {
    switch (type) {
        case TokenType::TYPE_TXT:  return DataType::TXT;
        case TokenType::TYPE_INT:  return DataType::INT;
        case TokenType::TYPE_DBL:  return DataType::DBL;
        case TokenType::TYPE_NAT:  return DataType::NAT;
        case TokenType::TYPE_BOOL: return DataType::BOOL;
        default:                   return DataType::NONE;
    }
}

Instruction Parser::parseFileDeclaration() {
    Token keywordToken = previous(); // Token 'File'

    if (!match(TokenType::IDENTIFIER)) {
        ErrorManager::logError(peek().line, "Se esperaba un nombre de identificador después de 'File'.");
        return { InstructionType::FILE_DECLARATION, "", "", "", DataType::NONE, 0, "", keywordToken.line, keywordToken.column };
    }
    std::string alias = previous().value;

    if (!match(TokenType::ASSIGN)) {
        ErrorManager::logError(peek().line, "Se esperaba '=' después del nombre del archivo '" + alias + "'.");
        return { InstructionType::FILE_DECLARATION, alias, "", "", DataType::NONE, 0, "", keywordToken.line, keywordToken.column };
    }

    if (!match(TokenType::STRING_LITERAL)) {
        ErrorManager::logError(peek().line, "Se esperaba una ruta entre comillas después de '='.");
        return { InstructionType::FILE_DECLARATION, alias, "", "", DataType::NONE, 0, "", keywordToken.line, keywordToken.column };
    }
    std::string path = previous().value;

    match(TokenType::SEMICOLON); // Consumir ';' opcional si existe

    return { InstructionType::FILE_DECLARATION, alias, path, "", DataType::NONE, 0, "", keywordToken.line, keywordToken.column };
}

Instruction Parser::parseRenderCommand() {
    Token keywordToken = previous(); // Token 'Render'

    if (!match(TokenType::LPAREN)) {
        ErrorManager::logError(peek().line, "Se esperaba '(' después de 'Render'.");
        return { InstructionType::RENDER_COMMAND, "", "", "", DataType::NONE, 0, "", keywordToken.line, keywordToken.column };
    }

    if (!match(TokenType::IDENTIFIER)) {
        ErrorManager::logError(peek().line, "Se esperaba el identificador del recurso dentro de 'Render(...)'.");
        return { InstructionType::RENDER_COMMAND, "", "", "", DataType::NONE, 0, "", keywordToken.line, keywordToken.column };
    }
    std::string alias = previous().value;

    if (!match(TokenType::RPAREN)) {
        ErrorManager::logError(peek().line, "Se esperaba ')' para cerrar la instrucción 'Render(" + alias + ")'.");
        return { InstructionType::RENDER_COMMAND, alias, "", "", DataType::NONE, 0, "", keywordToken.line, keywordToken.column };
    }

    match(TokenType::SEMICOLON); // Consumir ';' opcional si existe

    return { InstructionType::RENDER_COMMAND, alias, "", "", DataType::NONE, 0, "", keywordToken.line, keywordToken.column };
}

Instruction Parser::parseDataDeclaration() {
    Token keywordToken = previous(); // Token 'data'
    Instruction inst;
    inst.type = InstructionType::DATA_DECLARATION;
    inst.line = keywordToken.line;
    inst.column = keywordToken.column;

    // 1. Nombre de la variable
    if (!match(TokenType::IDENTIFIER)) {
        ErrorManager::logError(peek().line, "Se esperaba un nombre de variable después de 'data'.");
        return inst;
    }
    inst.varName = previous().value;

    // 2. Primer separador ':'
    if (!match(TokenType::COLON)) {
        ErrorManager::logError(peek().line, "Se esperaba ':' después del nombre de la variable '" + inst.varName + "'.");
        return inst;
    }

    // 3. Tipo de dato (txt, int, dbl, nat, bool)
    if (match(TokenType::TYPE_TXT) || match(TokenType::TYPE_INT) || 
        match(TokenType::TYPE_DBL) || match(TokenType::TYPE_NAT) || match(TokenType::TYPE_BOOL)) {
        inst.dataType = parseDataType(previous().type);
    } else {
        ErrorManager::logError(peek().line, "Tipo de dato no válido después de ':'. Se esperaba txt, int, dbl, nat o bool.");
        return inst;
    }

    // 4. Segundo separador ':'
    if (!match(TokenType::COLON)) {
        ErrorManager::logError(peek().line, "Se esperaba ':' después del tipo de dato.");
        return inst;
    }

    // 5. Tamaño en bytes (size)
    if (!match(TokenType::INT_LITERAL)) {
        ErrorManager::logError(peek().line, "Se esperaba un entero para especificar el tamaño en bytes.");
        return inst;
    }
    try {
        inst.byteSize = static_cast<size_t>(std::stoul(previous().value));
    } catch (...) {
        ErrorManager::logError(previous().line, "Tamaño en bytes no válido: '" + previous().value + "'");
        return inst;
    }

    // 6. Operador asignación '='
    if (!match(TokenType::ASSIGN)) {
        ErrorManager::logError(peek().line, "Se esperaba '=' después de especificar el tamaño en bytes.");
        return inst;
    }

    // 7. Valor asignado
    if (match(TokenType::STRING_LITERAL) || match(TokenType::INT_LITERAL) || 
        match(TokenType::FLOAT_LITERAL)  || match(TokenType::BOOL_LITERAL) || 
        match(TokenType::IDENTIFIER)) {
        inst.rawValue = previous().value;
    } else {
        ErrorManager::logError(peek().line, "Se esperaba un valor literal válido después de '='.");
        return inst;
    }

    // 8. Separador ';' opcional al final
    match(TokenType::SEMICOLON);

    return inst;
}

std::vector<Instruction> Parser::parse() {
    std::vector<Instruction> instructions;

    while (!isAtEnd()) {
        if (match(TokenType::KEYWORD_FILE)) {
            instructions.push_back(parseFileDeclaration());
        }
        else if (match(TokenType::KEYWORD_RENDER)) {
            instructions.push_back(parseRenderCommand());
        }
        else if (match(TokenType::KEYWORD_DATA)) {
            instructions.push_back(parseDataDeclaration());
        }
        else {
            Token badToken = advance();
            ErrorManager::logError(badToken.line, "Instrucción no válida o palabra no reconocida: '" + badToken.value + "'");
        }
    }

    return instructions;
}

} // namespace AphelG