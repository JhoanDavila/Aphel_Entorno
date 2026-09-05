#ifndef APHELG_AST_H
#define APHELG_AST_H

#include <string>
#include <cstddef>

namespace AphelG {

enum class InstructionType {
    FILE_DECLARATION,
    RENDER_COMMAND,
    DATA_DECLARATION   // Declaración de variables (data nombre : tipo : size = value)
};

enum class DataType {
    TXT,   // Cadenas de texto
    INT,   // Enteros con signo
    DBL,   // Números reales / punto flotante
    NAT,   // Números naturales (enteros positivos)
    BOOL,  // Booleanos
    NONE
};

struct Instruction {
    InstructionType type;
    
    // Campos para FILE_DECLARATION y RENDER_COMMAND
    std::string alias;        // Ej: "ventana"
    std::string path;         // Ej: "--;--;ui;Aphel_Ui;example.aphlui"

    // Campos para DATA_DECLARATION (data nombre : tipo : size = value)
    std::string varName;      // Nombre de la variable (ej: "usuario", "vidas")
    DataType dataType = DataType::NONE; // Tipo asignado (TXT, INT, DBL, NAT, BOOL)
    size_t byteSize = 0;      // Tamaño explícito en bytes (ej: 32, 4, 8, 2, 1)
    std::string rawValue;     // Valor literal inicial en formato texto para ser interpretado

    int line;
    int column;
};

} // namespace AphelG

#endif // APHELG_AST_H