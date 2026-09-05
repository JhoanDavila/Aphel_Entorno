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
        return { InstructionType::FILE_DECLARATION, "", "", "", DataType::NONE, "", keywordToken.line, keywordToken.column };
    }
    std::string alias = previous().value;

    if (!match(TokenType::ASSIGN)) {
        ErrorManager::logError(peek().line, "Se esperaba '=' después del nombre del archivo '" + alias + "'.");
        return { InstructionType::FILE_DECLARATION, alias, "", "", DataType::NONE, "", keywordToken.line, keywordToken.column };
    }

    if (!match(TokenType::STRING_LITERAL)) {
        ErrorManager::logError(peek().line, "Se esperaba una ruta entre comillas después de '='.");
        return { InstructionType::FILE_DECLARATION, alias, "", "", DataType::NONE, "", keywordToken.line, keywordToken.column };
    }
    std::string path = previous().value;

    match(TokenType::SEMICOLON);

    return { InstructionType::FILE_DECLARATION, alias, path, "", DataType::NONE, "", keywordToken.line, keywordToken.column };
}

Instruction Parser::parseRenderCommand() {
    Token keywordToken = previous(); // Token 'Render'

    if (!match(TokenType::LPAREN)) {
        ErrorManager::logError(peek().line, "Se esperaba '(' después de 'Render'.");
        return { InstructionType::RENDER_COMMAND, "", "", "", DataType::NONE, "", keywordToken.line, keywordToken.column };
    }

    if (!match(TokenType::IDENTIFIER)) {
        ErrorManager::logError(peek().line, "Se esperaba el identificador del recurso dentro de 'Render(...)'.");
        return { InstructionType::RENDER_COMMAND, "", "", "", DataType::NONE, "", keywordToken.line, keywordToken.column };
    }
    std::string alias = previous().value;

    if (!match(TokenType::RPAREN)) {
        ErrorManager::logError(peek().line, "Se esperaba ')' para cerrar la instrucción 'Render(" + alias + ")'.");
        return { InstructionType::RENDER_COMMAND, alias, "", "", DataType::NONE, "", keywordToken.line, keywordToken.column };
    }

    match(TokenType::SEMICOLON);

    return { InstructionType::RENDER_COMMAND, alias, "", "", DataType::NONE, "", keywordToken.line, keywordToken.column };
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

    // 2. Comprobar si especifica tipo explícito con ':'
    if (match(TokenType::COLON)) {
        if (match(TokenType::TYPE_TXT) || match(TokenType::TYPE_INT) || 
            match(TokenType::TYPE_DBL) || match(TokenType::TYPE_NAT) || match(TokenType::TYPE_BOOL)) {
            inst.dataType = parseDataType(previous().type);
        } else {
            ErrorManager::logError(peek().line, "Tipo de dato no válido después de ':'. Se esperaba txt, int, dbl, nat o bool.");
            return inst;
        }
    } else {
        inst.dataType = DataType::NONE; 
    }

    // 3. Operador de asignación '='
    if (!match(TokenType::ASSIGN)) {
        ErrorManager::logError(peek().line, "Se esperaba '=' después de la variable '" + inst.varName + "'.");
        return inst;
    }

    // 4. Inferencia o Validación del valor asignado
    Token valueToken = advance();
    
    if (inst.dataType == DataType::NONE) {
        switch (valueToken.type) {
            case TokenType::STRING_LITERAL: inst.dataType = DataType::TXT; break;
            case TokenType::INT_LITERAL:    inst.dataType = DataType::INT; break;
            case TokenType::FLOAT_LITERAL:  inst.dataType = DataType::DBL; break;
            case TokenType::BOOL_LITERAL:   inst.dataType = DataType::BOOL; break;
            default:
                ErrorManager::logError(valueToken.line, "No se pudo inferir el tipo de dato para '" + valueToken.value + "'.");
                return inst;
        }
    } else {
        bool isValidValue = false;

        switch (inst.dataType) {
            case DataType::TXT:
                isValidValue = (valueToken.type == TokenType::STRING_LITERAL);
                break;
            case DataType::INT:
                // Rechaza FLOAT_LITERAL estrictamente
                isValidValue = (valueToken.type == TokenType::INT_LITERAL);
                break;
            case DataType::DBL:
                isValidValue = (valueToken.type == TokenType::FLOAT_LITERAL || valueToken.type == TokenType::INT_LITERAL);
                break;
            case DataType::NAT:
                // Rechaza FLOAT_LITERAL estrictamente
                if (valueToken.type == TokenType::INT_LITERAL) {
                    try {
                        long long val = std::stoll(valueToken.value);
                        isValidValue = (val >= 0);
                    } catch (...) {
                        isValidValue = false;
                    }
                }
                break;
            case DataType::BOOL:
                isValidValue = (valueToken.type == TokenType::BOOL_LITERAL);
                break;
            default:
                isValidValue = false;
                break;
        }

        if (valueToken.type == TokenType::IDENTIFIER) {
            isValidValue = true;
        }

        if (!isValidValue) {
            ErrorManager::logError(
                valueToken.line, 
                "Error de Tipo: El valor '" + valueToken.value + "' no coincide con el tipo de dato especificado."
            );
            return inst;
        }
    }

    inst.rawValue = valueToken.value;
    match(TokenType::SEMICOLON);

    return inst;
}

Instruction Parser::parseVariableAssignment() {
    Token varToken = previous(); // Token IDENTIFIER
    Instruction inst;
    inst.type = InstructionType::VARIABLE_ASSIGNMENT;
    inst.varName = varToken.value;
    inst.line = varToken.line;
    inst.column = varToken.column;

    // 1. Comprobar si redefine tipo explícitamente con ':'
    if (match(TokenType::COLON)) {
        if (match(TokenType::TYPE_TXT) || match(TokenType::TYPE_INT) || 
            match(TokenType::TYPE_DBL) || match(TokenType::TYPE_NAT) || match(TokenType::TYPE_BOOL)) {
            inst.dataType = parseDataType(previous().type);
        } else {
            ErrorManager::logError(peek().line, "Tipo no válido en reasignación. Se esperaba txt, int, dbl, nat o bool.");
            return inst;
        }
    } else {
        inst.dataType = DataType::NONE; 
    }

    // 2. Operador '='
    if (!match(TokenType::ASSIGN)) {
        ErrorManager::logError(peek().line, "Se esperaba '=' en el reajuste de la variable '" + inst.varName + "'.");
        return inst;
    }

    // 3. Obtener el valor asignado y validar sintácticamente si hubo redeterminación de tipo
    Token valueToken = advance();

    if (inst.dataType != DataType::NONE) {
        bool isValidValue = false;

        switch (inst.dataType) {
            case DataType::TXT:
                isValidValue = (valueToken.type == TokenType::STRING_LITERAL);
                break;
            case DataType::INT:
                // Rechaza FLOAT_LITERAL estrictamente
                isValidValue = (valueToken.type == TokenType::INT_LITERAL);
                break;
            case DataType::DBL:
                isValidValue = (valueToken.type == TokenType::FLOAT_LITERAL || valueToken.type == TokenType::INT_LITERAL);
                break;
            case DataType::NAT:
                // Rechaza FLOAT_LITERAL estrictamente
                if (valueToken.type == TokenType::INT_LITERAL) {
                    try {
                        long long val = std::stoll(valueToken.value);
                        isValidValue = (val >= 0);
                    } catch (...) {
                        isValidValue = false;
                    }
                }
                break;
            case DataType::BOOL:
                isValidValue = (valueToken.type == TokenType::BOOL_LITERAL);
                break;
            default:
                isValidValue = false;
                break;
        }

        if (valueToken.type == TokenType::IDENTIFIER) {
            isValidValue = true;
        }

        if (!isValidValue) {
            ErrorManager::logError(
                valueToken.line, 
                "Error de Tipo en reasignación: El valor '" + valueToken.value + "' no coincide con el tipo especificado."
            );
            return inst;
        }
    }

    inst.rawValue = valueToken.value;
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
        else if (match(TokenType::IDENTIFIER)) { 
            instructions.push_back(parseVariableAssignment());
        }
        else {
            Token badToken = advance();
            ErrorManager::logError(badToken.line, "Instrucción no válida o palabra no reconocida: '" + badToken.value + "'");
        }
    }

    return instructions;
}

} // namespace AphelG