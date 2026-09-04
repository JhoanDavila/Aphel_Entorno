#include "ErrorManager.h"
#include <iostream>

namespace AphelG {

std::vector<SyntaxError> ErrorManager::errors;

void ErrorManager::logError(int line, const std::string& message) {
    errors.push_back({line, message});
}

bool ErrorManager::hasErrors() {
    return !errors.empty();
}

void ErrorManager::printErrors() {
    for (const auto& err : errors) {
        std::cerr << "[AphelG Error][Línea " << err.line << "] " << err.message << std::endl;
    }
}

void ErrorManager::clear() {
    errors.clear();
}

} // namespace AphelG