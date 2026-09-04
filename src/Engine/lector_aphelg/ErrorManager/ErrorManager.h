#ifndef APHELG_ERROR_MANAGER_H
#define APHELG_ERROR_MANAGER_H

#include <string>
#include <vector>

namespace AphelG {

struct SyntaxError {
    int line;
    std::string message;
};

class ErrorManager {
public:
    static std::vector<SyntaxError> errors;

    static void logError(int line, const std::string& message);
    static bool hasErrors();
    static void printErrors();
    static void clear();
};

} // namespace AphelG

#endif