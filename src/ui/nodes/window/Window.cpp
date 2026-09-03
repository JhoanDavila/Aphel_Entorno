#include "ui/nodes/window/Window.h"

WindowNode::WindowNode(const std::string& nodeName, Node* parentNode)
    : Space2D(nodeName, parentNode), title("Aphel Window"), backgroundColor{255, 255, 255} {}