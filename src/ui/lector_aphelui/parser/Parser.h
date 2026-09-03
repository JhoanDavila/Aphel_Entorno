#ifndef PARSER_H
#define PARSER_H

#include "Token.h"
#include "Node.h"
#include <vector>
#include <memory>

class Parser {
public:
    // Recibe los tokens y construye el árbol jerárquico de Nodos
    static std::shared_ptr<Node> parse(const std::vector<Token>& tokens);
};

#endif // PARSER_H