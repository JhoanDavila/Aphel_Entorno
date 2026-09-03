#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include <memory>

class Node {
public:
    std::string name;
    Node* parent;
    std::vector<std::shared_ptr<Node>> children;

    // Constructor que inicializa el nombre y coloca el padre en nullptr por defecto
    explicit Node(const std::string& nodeName, Node* parentNode = nullptr);

    // Método para vincular un hijo y asignarle este nodo como su padre
    void addChild(std::shared_ptr<Node> child);
};

#endif // NODE_H