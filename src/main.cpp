#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Engine/lector_aphelg/Cleaner/Cleaner.h"
#include "Engine/lector_aphelg/Tokenizer/Tokenizer.h"
#include "Engine/lector_aphelg/Parser/Parser.h"
#include "Engine/lector_aphelg/PathResolver/PathResolver.h"
#include "Engine/lector_aphelg/ErrorManager/ErrorManager.h"

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

    // 1. Limpieza de comentarios
    std::string cleanedScript = AphlgCleaner::clean(buffer.str());

    // 2. Tokenización
    AphelG::Tokenizer tokenizer(cleanedScript);
    std::vector<AphelG::Token> tokens = tokenizer.tokenize();

    // 3. Análisis Sintáctico (Parser)
    AphelG::Parser parser(tokens);
    std::vector<AphelG::Instruction> ast = parser.parse();

    // 4. Reporte de Errores o Ejecución del AST
    if (AphelG::ErrorManager::hasErrors()) {
        std::cout << "--- ERRORES ENCONTRADOS ---" << std::endl;
        AphelG::ErrorManager::printErrors();
        return 1;
    }

    std::cout << "--- INSTRUCCIONES VALIDADAS (AST) ---" << std::endl;
    for (const auto& inst : ast) {
        if (inst.type == AphelG::InstructionType::FILE_DECLARATION) {
            std::string realPath = AphelG::PathResolver::resolve(inst.path);
            std::cout << "[FILE] Alias: " << inst.alias 
                      << " | Ruta SO: " << realPath << std::endl;
        } 
        else if (inst.type == AphelG::InstructionType::RENDER_COMMAND) {
            std::cout << "[RENDER] Target: " << inst.alias << std::endl;
        }
    }

    return 0;
}