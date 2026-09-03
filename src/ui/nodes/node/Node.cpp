#include "Node.h"

Node::Node(const std::string& nodeName, Node* parentNode)
    : name(nodeName), parent(parentNode) {}

void Node::addChild(std::shared_ptr<Node> child) {
    if (child) {
        child->parent = this; // Asignamos este nodo como padre del hijo
        children.push_back(child); // Lo agregamos a la lista de hijos
    }
}