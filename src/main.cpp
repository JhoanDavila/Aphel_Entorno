#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// Incluimos las cabeceras de lector_aphelg
#include "Engine/lector_aphelg/Cleaner/Cleaner.h"
#include "Engine/lector_aphelg/Tokenizer/Tokenizer.h"
#include "Engine/lector_aphelg/ErrorManager/ErrorManager.h"

// Función auxiliar para imprimir el tipo de Token en texto
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

    // 1. Limpieza del código
    std::string cleanedScript = AphlgCleaner::clean(buffer.str());

    std::cout << "--- CODIGO LIMPIO (.aphlg) ---" << std::endl;
    std::cout << cleanedScript << std::endl;
    std::cout << "-------------------------------" << std::endl;

    // 2. Tokenización
    AphelG::Tokenizer tokenizer(cleanedScript);
    std::vector<AphelG::Token> tokens = tokenizer.tokenize();

    std::cout << "\n--- LISTA DE TOKENS OBTENIDOS ---" << std::endl;
    for (const auto& token : tokens) {
        std::cout << "[Linea " << token.line << ", Col " << token.column << "] "
                  << "Tipo: " << tokenTypeToString(token.type)
                  << " | Valor: \"" << token.value << "\"" << std::endl;
    }
    std::cout << "---------------------------------" << std::endl;

    // 3. Verificación de errores léxicos acumulados
    if (AphelG::ErrorManager::hasErrors()) {
        std::cout << "\nSe encontraron errores durante la tokenizacion:" << std::endl;
        AphelG::ErrorManager::printErrors();
    } else {
        std::cout << "\nTokenizacion completada exitosamente sin errores." << std::endl;
    }

    return 0;
}