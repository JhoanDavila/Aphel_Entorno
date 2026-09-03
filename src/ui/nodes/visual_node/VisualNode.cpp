#include "ui/nodes/visual_node/VisualNode.h"

VisualNode::VisualNode(const std::string& nodeType, const std::string& nodeName, Node* parentNode)
    : Node(nodeType, nodeName, parentNode), position{0, 0} {}