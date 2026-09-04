#ifndef APHELG_AST_H
#define APHELG_AST_H

#include <string>

namespace AphelG {

enum class InstructionType {
    FILE_DECLARATION,
    RENDER_COMMAND
};

struct Instruction {
    InstructionType type;
    std::string alias;       // Ej: "ventana"
    std::string path;        // Ej: "--;--;ui;Aphel_Ui;example.aphlui" (solo en FILE_DECLARATION)
    int line;
    int column;
};

} // namespace AphelG

#endif // APHELG_AST_H