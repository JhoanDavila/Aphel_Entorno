#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "Token.h"
#include <string>
#include <vector>

class Tokenizer {
public:
    // Convierte el vector de líneas limpias en una lista plana de Tokens
    static std::vector<Token> tokenize(const std::vector<std::string>& cleanLines);
};

#endif // TOKENIZER_H