#ifndef ERROR_MANAGER_H
#define ERROR_MANAGER_H

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
    static void logError(int line, const std::string& message);
    static bool hasErrors();
    static void printErrors();
    static void clear();
};

#endif // ERROR_MANAGER_H