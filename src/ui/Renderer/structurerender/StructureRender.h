#pragma once

#include <vector>
#include <memory>
#include <iostream>

// Incluimos la clase base de tus nodos
#include "ui/nodes/node/Node.h"

// Un vértice representa un punto en pantalla con posición y color
struct Vertex {
    float position[2]; // X, Y
    float color[4];    // R, G, B, A (de 0.0 a 1.0)
};

// El paquete con todos los vértices e índices generados para la GPU
struct RenderBatch {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};

class StructureRender {
private:
    RenderBatch batch;

    void processNode(const std::shared_ptr<Node>& node, float parentX, float parentY);
    void generateQuad(float x, float y, float w, float h);

public:
    StructureRender() = default;

    // Método principal: toma el nodo raíz y devuelve el lote geométrico
    RenderBatch buildGeometry(const std::shared_ptr<Node>& root);

};