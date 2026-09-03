#ifndef CLEANER_H
#define CLEANER_H

#include <string>
#include <vector>

class Cleaner {
public:
    // Recibe el contenido raw del archivo en líneas y retorna solo las líneas válidas de código
    static std::vector<std::string> removeCommentsAndEmptyLines(const std::vector<std::string>& rawLines);
};

#endif // CLEANER_H