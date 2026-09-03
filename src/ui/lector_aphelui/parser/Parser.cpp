#include "Parser.h"
#include <stack>

std::shared_ptr<Node> Parser::parse(const std::vector<Token>& tokens) {
    if (tokens.empty()) return nullptr;

    std::shared_ptr<Node> root = nullptr;
    std::stack<std::pair<int, std::shared_ptr<Node>>> nodeStack;

    for (size_t i = 0; i < tokens.size(); ++i) {
        const auto& token = tokens[i];

        // Cuando encontramos un nuevo Nodo
        if (token.type == TokenType::NODE_TYPE) {
            auto currentNode = std::make_shared<Node>(token.value);

            // Si el siguiente token es la propiedad .name, le asignamos su nombre
            if (i + 2 < tokens.size() && 
                tokens[i + 1].type == TokenType::PROPERTY && 
                tokens[i + 1].value == ".name" && 
                tokens[i + 2].type == TokenType::VALUE) {
                
                currentNode->name = tokens[i + 2].value;
            }

            // Si es el primer nodo, se convierte en la Raíz
            if (!root) {
                root = currentNode;
                nodeStack.push({token.indentLevel, currentNode});
                continue;
            }

            // Desapilamos nodos que estén al mismo nivel o más profundos
            while (!nodeStack.empty() && nodeStack.top().first >= token.indentLevel) {
                nodeStack.pop();
            }

            // El nodo en el tope de la pila es el padre directo
            if (!nodeStack.empty()) {
                nodeStack.top().second->addChild(currentNode);
            }

            // Apilamos el nodo actual para futuros hijos
            nodeStack.push({token.indentLevel, currentNode});
        }
    }

    return root;
}