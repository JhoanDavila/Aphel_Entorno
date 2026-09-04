#include "StructureRender.h"

// Encabezados necesarios de la jerarquía de nodos
#include "ui/nodes/visual_node/VisualNode.h"
#include "ui/nodes/2dspace/2DSpace.h"
#include "ui/nodes/window/Window.h"

RenderBatch StructureRender::buildGeometry(const std::shared_ptr<Node>& root) {
    batch.vertices.clear();
    batch.indices.clear();

    if (root) {
        // Comenzamos el recorrido recursivo desde el origen (0,0)
        processNode(root, 0.0f, 0.0f);
    }

    return batch;
}

void StructureRender::processNode(const std::shared_ptr<Node>& node, float parentX, float parentY) {
    if (!node) return;

    float currentX = parentX;
    float currentY = parentY;

    // 1. Si el nodo hereda de VisualNode, acumulamos su posición relativa
    auto visualNode = std::dynamic_pointer_cast<VisualNode>(node);
    if (visualNode) {
        currentX += visualNode->position.x;
        currentY += visualNode->position.y;
    }

    // 2. Si hereda de Space2D (o Window), tiene tamaño -> generamos el rectángulo
    auto spaceNode = std::dynamic_pointer_cast<Space2D>(node);
    if (spaceNode) {
        generateQuad(currentX, currentY, spaceNode->size.width, spaceNode->size.height);
    }

    // 3. Recorrer recursivamente todos los hijos del nodo actual
    for (const auto& child : node->children) {
        processNode(child, currentX, currentY);
    }
}

void StructureRender::generateQuad(float x, float y, float w, float h) {
    unsigned int baseIndex = static_cast<unsigned int>(batch.vertices.size());

    // 4 vértices para formar un rectángulo (X, Y, R, G, B, A)
    batch.vertices.push_back({{ x,     y     }, { 1.0f, 1.0f, 1.0f, 1.0f }});
    batch.vertices.push_back({{ x + w, y     }, { 1.0f, 1.0f, 1.0f, 1.0f }});
    batch.vertices.push_back({{ x + w, y + h }, { 1.0f, 1.0f, 1.0f, 1.0f }});
    batch.vertices.push_back({{ x,     y + h }, { 1.0f, 1.0f, 1.0f, 1.0f }});

    // 6 índices que forman los 2 triángulos del rectángulo
    batch.indices.push_back(baseIndex + 0);
    batch.indices.push_back(baseIndex + 1);
    batch.indices.push_back(baseIndex + 2);

    batch.indices.push_back(baseIndex + 2);
    batch.indices.push_back(baseIndex + 3);
    batch.indices.push_back(baseIndex + 0);
}

void StructureRender::debugPrintBatch(const RenderBatch& b) const {
    std::cout << "==========================================" << std::endl;
    std::cout << "[StructureRender] Geometria Generada:" << std::endl;
    std::cout << "  - Total Vertices: " << b.vertices.size() << std::endl;
    std::cout << "  - Total Indices:  " << b.indices.size() << " (" << b.indices.size() / 6 << " rectangulos)" << std::endl;
    
    for (size_t i = 0; i < b.vertices.size(); i += 4) {
        std::cout << "  Rectangulo " << (i / 4) + 1 << " -> Pos: (" 
                  << b.vertices[i].position[0] << ", " << b.vertices[i].position[1] << ")" << std::endl;
    }
    std::cout << "==========================================" << std::endl;
}