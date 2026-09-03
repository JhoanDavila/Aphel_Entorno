#ifndef VISUAL_NODE_H
#define VISUAL_NODE_H

#include "Node.h"

struct Vector2D {
    int x = 0;
    int y = 0;
};

class VisualNode : public Node {
public:
    Vector2D position;

    // Hereda los constructores del Node base
    explicit VisualNode(const std::string& nodeName, Node* parentNode = nullptr);
};

#endif // VISUAL_NODE_H