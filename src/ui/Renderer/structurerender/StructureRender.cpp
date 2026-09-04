#include "StructureRender.h"

// Encabezados de la jerarquía de nodos
#include "ui/nodes/visual_node/VisualNode.h"
#include "ui/nodes/2dspace/2DSpace.h"
#include "ui/nodes/window/Window.h"

RenderBatch StructureRender::buildGeometry(const std::shared_ptr<Node>& root) {
    batch.vertices.clear();
    batch.indices.clear();

    if (root) {
        processNode(root, 0.0f, 0.0f);
    }

    return batch;
}

void StructureRender::processNode(const std::shared_ptr<Node>& node, float parentX, float parentY) {
    if (!node) return;

    float currentX = parentX;
    float currentY = parentY;

    // 1. Si es VisualNode, sumamos la posición relativa
    auto visualNode = std::dynamic_pointer_cast<VisualNode>(node);
    if (visualNode) {
        currentX += visualNode->position.x;
        currentY += visualNode->position.y;
    }

    // 2. Si es Space2D (o Window), genera sus 4 vértices y 6 índices
    auto spaceNode = std::dynamic_pointer_cast<Space2D>(node);
    if (spaceNode) {
        generateQuad(currentX, currentY, spaceNode->size.width, spaceNode->size.height);
    }

    // 3. Recorrer los hijos recursivamente
    for (const auto& child : node->children) {
        processNode(child, currentX, currentY);
    }
}

void StructureRender::generateQuad(float x, float y, float w, float h) {
    unsigned int baseIndex = static_cast<unsigned int>(batch.vertices.size());

    // 4 vértices del rectángulo (X, Y, R, G, B, A)
    batch.vertices.push_back({{ x,     y     }, { 1.0f, 1.0f, 1.0f, 1.0f }});
    batch.vertices.push_back({{ x + w, y     }, { 1.0f, 1.0f, 1.0f, 1.0f }});
    batch.vertices.push_back({{ x + w, y + h }, { 1.0f, 1.0f, 1.0f, 1.0f }});
    batch.vertices.push_back({{ x,     y + h }, { 1.0f, 1.0f, 1.0f, 1.0f }});

    // 2 triángulos (6 índices)
    batch.indices.push_back(baseIndex + 0);
    batch.indices.push_back(baseIndex + 1);
    batch.indices.push_back(baseIndex + 2);

    batch.indices.push_back(baseIndex + 2);
    batch.indices.push_back(baseIndex + 3);
    batch.indices.push_back(baseIndex + 0);
}
