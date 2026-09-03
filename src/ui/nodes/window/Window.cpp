#include "ui/nodes/window/Window.h"

WindowNode::WindowNode(const std::string& nodeType, const std::string& nodeName, Node* parentNode)
    : Space2D(nodeType, nodeName, parentNode), title("Untitled Window"), backgroundColor{} {}