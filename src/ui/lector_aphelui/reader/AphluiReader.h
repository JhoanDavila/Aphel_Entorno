#ifndef APHLUI_READER_H
#define APHLUI_READER_H

#include "ui/nodes/node/Node.h"
#include <string>
#include <memory>

class AphluiReader {
public:
    // Toma la ruta del archivo .aphlui, ejecuta la canalización completa y retorna el nodo raíz.
    static std::shared_ptr<Node> loadFromFile(const std::string& filePath);
};

#endif // APHLUI_READER_H