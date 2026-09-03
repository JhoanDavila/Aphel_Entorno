#include "ui/nodes/node/Node.h"

Node::Node(const std::string& nodeType, const std::string& nodeName, Node* parentNode)
    : type(nodeType), name(nodeName), parent(parentNode) {}

void Node::addChild(std::shared_ptr<Node> child) {
    if (child) {
        child->parent = this;
        children.push_back(child);
    }
}