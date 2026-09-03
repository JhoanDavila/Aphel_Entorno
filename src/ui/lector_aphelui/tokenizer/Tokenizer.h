#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "ui/lector_aphelui/tokenizer/Token.h"
#include <string>
#include <vector>

class Tokenizer {
public:
    // Convierte las líneas limpias en una lista plana de Tokens con información de línea e indentación
    static std::vector<Token> tokenize(const std::vector<std::string>& cleanLines);
};

#endif // TOKENIZER_H