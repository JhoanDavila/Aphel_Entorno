#include "ui/lector_aphelui/reader/AphluiReader.h"
#include "ui/lector_aphelui/cleaner/Cleaner.h"
#include "ui/lector_aphelui/tokenizer/Tokenizer.h"
#include "ui/lector_aphelui/parser/Parser.h"

#include <fstream>
#include <iostream>
#include <vector>

std::shared_ptr<Node> AphluiReader::loadFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "[AphluiReader Error] No se pudo abrir el archivo: " << filePath << std::endl;
        return nullptr;
    }

    std::vector<std::string> rawLines;
    std::string line;
    while (std::getline(file, line)) {
        rawLines.push_back(line);
    }
    file.close();

    // Pipeline interno de lectura y parsing
    std::vector<std::string> cleanLines = Cleaner::removeCommentsAndEmptyLines(rawLines);
    std::vector<Token> tokens = Tokenizer::tokenize(cleanLines);
    
    return Parser::parse(tokens);
}