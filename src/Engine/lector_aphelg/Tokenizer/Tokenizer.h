#ifndef APHELG_TOKENIZER_H
#define APHELG_TOKENIZER_H

#include "Token.h"
#include <string>
#include <vector>

namespace AphelG {

class Tokenizer {
private:
    std::string source;
    size_t index;
    int line;
    int column;

    // Métodos auxiliares de recorrido
    char peek() const;
    char advance();
    bool isAtEnd() const;
    void skipWhitespace();

    // Generadores de tokens específicos
    Token makeString();
    Token makeIdentifierOrKeyword();

public:
    explicit Tokenizer(const std::string& sourceCode);

    // Método principal que convierte el texto en la lista de tokens
    std::vector<Token> tokenize();
};

} // namespace AphelG

#endif // APHELG_TOKENIZER_H