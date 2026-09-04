#ifndef APHELG_PARSER_H
#define APHELG_PARSER_H

#include "../Tokenizer/Token.h"
#include "AST.h"
#include <vector>

namespace AphelG {

class Parser {
private:
    std::vector<Token> tokens;
    size_t current;

    // Métodos auxiliares de inspección
    const Token& peek() const;
    const Token& previous() const;
    Token advance();
    bool check(TokenType type) const;
    bool match(TokenType type);
    bool isAtEnd() const;

    // Métodos de análisis sintáctico (Gramática)
    Instruction parseFileDeclaration();
    Instruction parseRenderCommand();

public:
    explicit Parser(std::vector<Token> tokens);

    // Transforma la lista de tokens en la secuencia de instrucciones
    std::vector<Instruction> parse();
};

} // namespace AphelG

#endif // APHELG_PARSER_H