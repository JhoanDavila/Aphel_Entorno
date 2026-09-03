#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include <memory>

class Node {
protected:
    std::string type;

public:
    std::string name;
    Node* parent;
    std::vector<std::shared_ptr<Node>> children;

    // Constructores: permiten inicializar tanto el nombre como el tipo de nodo
    explicit Node(const std::string& nodeType, const std::string& nodeName = "", Node* parentNode = nullptr);

    // Destructor virtual necesario para el polimorfismo y RTTI
    virtual ~Node() = default;

    // Getter del tipo de nodo (satisface la llamada node->getType())
    std::string getType() const { return type; }

    void addChild(std::shared_ptr<Node> child);
};

#endif // NODE_H