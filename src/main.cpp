#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// Incluimos las cabeceras de lector_aphelg
#include "Engine/lector_aphelg/Cleaner/Cleaner.h"
#include "Engine/lector_aphelg/Tokenizer/Tokenizer.h"
#include "Engine/lector_aphelg/Parser/Parser.h"
#include "Engine/lector_aphelg/ErrorManager/ErrorManager.h"

std::string tokenTypeToString(AphelG::TokenType type) {
    switch (type) {
        case AphelG::TokenType::KEYWORD_FILE:   return "KEYWORD_FILE";
        case AphelG::TokenType::KEYWORD_RENDER: return "KEYWORD_RENDER";
        case AphelG::TokenType::ASSIGN:         return "ASSIGN";
        case AphelG::TokenType::LPAREN:         return "LPAREN";
        case AphelG::TokenType::RPAREN:         return "RPAREN";
        case AphelG::TokenType::IDENTIFIER:     return "IDENTIFIER";
        case AphelG::TokenType::STRING_LITERAL: return "STRING_LITERAL";
        case AphelG::TokenType::END_OF_FILE:    return "END_OF_FILE";
        case AphelG::TokenType::UNKNOWN:        return "UNKNOWN";
        default:                                return "OTHER";
    }
}

std::string instructionTypeToString(AphelG::InstructionType type) {
    switch (type) {
        case AphelG::InstructionType::FILE_DECLARATION: return "FILE_DECLARATION";
        case AphelG::InstructionType::RENDER_COMMAND:   return "RENDER_COMMAND";
        default:                                         return "UNKNOWN";
    }
}

int main() {
    std::string filePath = "/home/jhoan/personal_deloved/Aphel_Entorno/src/Engine/Aphel_Language/example.aphlg";
    
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << filePath << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    // 1. Limpieza
    std::string cleanedScript = AphlgCleaner::clean(buffer.str());

    // 2. Tokenización
    AphelG::Tokenizer tokenizer(cleanedScript);
    std::vector<AphelG::Token> tokens = tokenizer.tokenize();

    // 3. Análisis Sintáctico (Parser)
    AphelG::Parser parser(tokens);
    std::vector<AphelG::Instruction> ast = parser.parse();

    // 4. Verificación e impresión de errores
    if (AphelG::ErrorManager::hasErrors()) {
        std::cout << "\n--- ERRORES ENCONTRADOS ---" << std::endl;
        AphelG::ErrorManager::printErrors();
        std::cout << "---------------------------" << std::endl;
    } else {
        std::cout << "\n--- ARBOL DE INSTRUCCIONES GENERADO (AST) ---" << std::endl;
        for (const auto& inst : ast) {
            std::cout << "[Linea " << inst.line << "] Tipo: " << instructionTypeToString(inst.type)
                      << " | Alias: \"" << inst.alias << "\"";
            if (inst.type == AphelG::InstructionType::FILE_DECLARATION) {
                std::cout << " | Path: \"" << inst.path << "\"";
            }
            std::cout << std::endl;
        }
        std::cout << "---------------------------------------------" << std::endl;
        std::cout << "Analisis completado exitosamente sin errores sintacticos." << std::endl;
    }

    return 0;
}