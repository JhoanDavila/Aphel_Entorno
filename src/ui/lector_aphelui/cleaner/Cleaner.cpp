#include "Cleaner.h"

std::vector<std::string> Cleaner::removeCommentsAndEmptyLines(const std::vector<std::string>& rawLines) {
    std::vector<std::string> cleanLines;

    for (const auto& line : rawLines) {
        // 1. Buscar la primera aparición del carácter '#'
        size_t commentPos = line.find('#');
        
        // 2. Extraer solo el texto antes del '#' (si existía)
        std::string codePart = (commentPos != std::string::npos) ? line.substr(0, commentPos) : line;

        // 3. Verificar si la línea restante tiene contenido real (no solo espacios en blanco)
        size_t firstNonSpace = codePart.find_first_not_of(" \t\r\n");
        if (firstNonSpace != std::string::npos) {
            cleanLines.push_back(codePart);
        }
    }

    return cleanLines;
}