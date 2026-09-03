#include "ui/lector_aphelui/reader/AphluiReader.h"
#include "ui/lector_aphelui/cleaner/Cleaner.h"
#include "ui/lector_aphelui/tokenizer/Tokenizer.h"
#include "ui/lector_aphelui/parser/Parser.h"
#include "ui/lector_aphelui/error_manager/ErrorManager.h"

#include <fstream>
#include <iostream>
#include <vector>

std::shared_ptr<Node> AphluiReader::loadFromFile(const std::string& filePath) {
    // 1. Reiniciar el registro de errores de ejecuciones previas
    ErrorManager::clear();

    std::ifstream file(filePath);
    if (!file.is_open()) {
        ErrorManager::logError(0, "Could not open file: " + filePath);
        ErrorManager::printErrors();
        return nullptr;
    }

    std::vector<std::string> rawLines;
    std::string line;
    while (std::getline(file, line)) {
        rawLines.push_back(line);
    }
    file.close();

    // 2. Limpieza de comentarios y líneas vacías
    std::vector<std::string> cleanLines = Cleaner::removeCommentsAndEmptyLines(rawLines);

    // 3. Tokenización y validación de sintaxis
    std::vector<Token> tokens = Tokenizer::tokenize(cleanLines);

    // Si el Tokenizer encontró caracteres no válidos o sintaxis errónea, frenamos
    if (ErrorManager::hasErrors()) {
        std::cerr << "[AphluiReader] Build aborted due to syntax errors." << std::endl;
        ErrorManager::printErrors();
        return nullptr;
    }

    // 4. Generación del árbol de nodos
    std::shared_ptr<Node> root = Parser::parse(tokens);

    // Si el Parser generó algún error durante la construcción
    if (ErrorManager::hasErrors()) {
        std::cerr << "[AphluiReader] Build aborted due to parsing errors." << std::endl;
        ErrorManager::printErrors();
        return nullptr;
    }

    return root;
}