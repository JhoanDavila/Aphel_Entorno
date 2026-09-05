#ifndef APHELG_AST_H
#define APHELG_AST_H

#include <string>

namespace AphelG {

enum class InstructionType {
    FILE_DECLARATION,
    RENDER_COMMAND,
    DATA_DECLARATION,
    VARIABLE_ASSIGNMENT // <--- Reajuste de variable existente
};

enum class DataType {
    TXT,
    INT,
    DBL,
    NAT,
    BOOL,
    NONE
};

struct Instruction {
    InstructionType type;
    std::string alias;      // Nombre de recurso o alias de archivo
    std::string path;       // Ruta al archivo .aphlui
    std::string varName;    // Nombre de variable
    DataType dataType;      // Tipo de dato
    std::string rawValue;   // Valor asignado en texto
    int line;
    int column;
};

} // namespace AphelG

#endif // APHELG_AST_H