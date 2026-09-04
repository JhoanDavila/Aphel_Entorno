#include "Parser.h"
#include "../ErrorManager/ErrorManager.h"

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

Instruction Parser::parseFileDeclaration() {
    Token keywordToken = previous(); // Token 'File'

    if (!match(TokenType::IDENTIFIER)) {
        ErrorManager::logError(peek().line, "Se esperaba un nombre de identificador después de 'File'.");
        return { InstructionType::FILE_DECLARATION, "", "", keywordToken.line, keywordToken.column };
    }
    std::string alias = previous().value;

    if (!match(TokenType::ASSIGN)) {
        ErrorManager::logError(peek().line, "Se esperaba '=' después del nombre del archivo '" + alias + "'.");
        return { InstructionType::FILE_DECLARATION, alias, "", keywordToken.line, keywordToken.column };
    }

    if (!match(TokenType::STRING_LITERAL)) {
        ErrorManager::logError(peek().line, "Se esperaba una ruta entre comillas despues de '='.");
        return { InstructionType::FILE_DECLARATION, alias, "", keywordToken.line, keywordToken.column };
    }
    std::string path = previous().value;

    return { InstructionType::FILE_DECLARATION, alias, path, keywordToken.line, keywordToken.column };
}

Instruction Parser::parseRenderCommand() {
    Token keywordToken = previous(); // Token 'Render'

    if (!match(TokenType::LPAREN)) {
        ErrorManager::logError(peek().line, "Se esperaba '(' después de 'Render'.");
        return { InstructionType::RENDER_COMMAND, "", "", keywordToken.line, keywordToken.column };
    }

    if (!match(TokenType::IDENTIFIER)) {
        ErrorManager::logError(peek().line, "Se esperaba el identificador del recurso dentro de 'Render(...)'.");
        return { InstructionType::RENDER_COMMAND, "", "", keywordToken.line, keywordToken.column };
    }
    std::string alias = previous().value;

    if (!match(TokenType::RPAREN)) {
        ErrorManager::logError(peek().line, "Se esperaba ')' para cerrar la instrucción 'Render(" + alias + ")'.");
        return { InstructionType::RENDER_COMMAND, alias, "", keywordToken.line, keywordToken.column };
    }

    return { InstructionType::RENDER_COMMAND, alias, "", keywordToken.line, keywordToken.column };
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
        else {
            Token badToken = advance();
            ErrorManager::logError(badToken.line, "Instrucción no válida o palabra no reconocida: '" + badToken.value + "'");
        }
    }

    return instructions;
}

} // namespace AphelG