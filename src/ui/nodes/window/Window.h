#ifndef WINDOW_NODE_H
#define WINDOW_NODE_H

#include "ui/nodes/2dspace/2DSpace.h"
#include <string>

struct Color {
    unsigned char r = 255;
    unsigned char g = 255;
    unsigned char b = 255;
    unsigned char a = 255; // Soporte para transparencia (0 = invisible, 255 = opaco)

    // Útil para cuando le pasemos datos a la GPU (OpenGL usa floats de 0.0f a 1.0f)
    float rFloat() const { return r / 255.0f; }
    float gFloat() const { return g / 255.0f; }
    float bFloat() const { return b / 255.0f; }
    float aFloat() const { return a / 255.0f; }
};

class WindowNode : public Space2D {
public:
    std::string title;
    Color backgroundColor;

    explicit WindowNode(const std::string& nodeName, Node* parentNode = nullptr);
};

#endif // WINDOW_NODE_H