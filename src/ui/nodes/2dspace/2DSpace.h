#ifndef SPACE2D_H
#define SPACE2D_H

#include "ui/nodes/visual_node/VisualNode.h"

struct Size2D {
    int width = 0;
    int height = 0;
};

class Space2D : public VisualNode {
public:
    Size2D size;

    explicit Space2D(const std::string& nodeName, Node* parentNode = nullptr);
};

#endif // SPACE2D_H