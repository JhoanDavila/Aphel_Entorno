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

    // Destructor virtual necesario para habilitar el polimorfismo y dynamic_pointer_cast
    virtual ~Node() = default;

    explicit Node(const std::string& nodeName, Node* parentNode = nullptr);

    void addChild(std::shared_ptr<Node> child);
};

#endif // NODE_H