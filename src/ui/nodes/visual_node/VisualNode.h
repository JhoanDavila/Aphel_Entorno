#ifndef VISUAL_NODE_H
#define VISUAL_NODE_H

#include "../node/Node.h"

struct Vector2D {
    int x = 0;
    int y = 0;
};

class VisualNode : public Node {
public:
    Vector2D position;

    // Acepta el tipo de nodo (por defecto "VisualNode") y lo pasa al nodo base
    explicit VisualNode(const std::string& nodeType = "VisualNode", 
                        const std::string& nodeName = "", 
                        Node* parentNode = nullptr);
};

#endif // VISUAL_NODE_H