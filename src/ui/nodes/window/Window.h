#ifndef WINDOW_NODE_H
#define WINDOW_NODE_H

#include "ui/nodes/2dspace/2DSpace.h"
#include <string>

struct ColorRGB {
    unsigned char r = 255;
    unsigned char g = 255;
    unsigned char b = 255;
};

class WindowNode : public Space2D {
public:
    std::string title;
    ColorRGB backgroundColor;

    explicit WindowNode(const std::string& nodeName, Node* parentNode = nullptr);
};

#endif // WINDOW_NODE_H