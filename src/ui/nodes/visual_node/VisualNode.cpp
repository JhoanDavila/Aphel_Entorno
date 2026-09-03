#include "VisualNode.h"

VisualNode::VisualNode(const std::string& nodeName, Node* parentNode)
    : Node(nodeName, parentNode), position{0, 0} {}