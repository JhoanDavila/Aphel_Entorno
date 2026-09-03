#include "ui/lector_aphelui/error_manager/ErrorManager.h"
#include <iostream>

std::vector<SyntaxError> ErrorManager::errors;

void ErrorManager::logError(int line, const std::string& message) {
    errors.push_back({line, message});
}

bool ErrorManager::hasErrors() {
    return !errors.empty();
}

void ErrorManager::printErrors() {
    for (const auto& err : errors) {
        std::cerr << "[AphelUI Syntax Error] Line " << err.line << ": " << err.message << std::endl;
    }
}

void ErrorManager::clear() {
    errors.clear();
}