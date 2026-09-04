#ifndef APHLG_ERROR_MANAGER_H
#define APHLG_ERROR_MANAGER_H

#include <string>
#include <vector>

struct SyntaxError {
    int line;
    std::string message;
};

class ErrorManager {
private:
    static std::vector<SyntaxError> errors;

public:
    // Registra un nuevo error indicando la línea y el mensaje
    static void logError(int line, const std::string& message);

    // Indica si existen errores registrados
    static bool hasErrors();

    // Muestra en consola (std::cerr) todos los errores acumulados
    static void printErrors();

    // Vacía la lista de errores registrados
    static void clear();
};

#endif // APHLG_ERROR_MANAGER_H