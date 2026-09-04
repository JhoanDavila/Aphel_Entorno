#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// Incluimos la cabecera del Cleaner de AphelG
#include "Engine/lector_aphelg/Cleaner/Cleaner.h"

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

    std::string rawScript = buffer.str();

    // Procesamos el código eliminando los comentarios
    std::string cleanedScript = AphlgCleaner::clean(rawScript);

    std::cout << "--- CODIGO LIMPIO (.aphlg) ---" << std::endl;
    std::cout << cleanedScript << std::endl;
    std::cout << "-------------------------------" << std::endl;

    return 0;
}