#include "ui/nodes/2dspace/2DSpace.h"

Space2D::Space2D(const std::string& nodeType, const std::string& nodeName, Node* parentNode)
    : VisualNode(nodeType, nodeName, parentNode), size{0, 0} {}