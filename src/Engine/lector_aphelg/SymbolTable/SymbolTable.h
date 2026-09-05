#ifndef APHELG_SYMBOL_TABLE_H
#define APHELG_SYMBOL_TABLE_H

#include "../Parser/AST.h"
#include <string>
#include <unordered_map>

namespace AphelG {

struct Variable {
    std::string name;
    DataType type;
    std::string stringVal;
    long long intVal = 0;
    double dblVal = 0.0;
    unsigned long long natVal = 0;
    bool boolVal = false;
};

class SymbolTable {
private:
    std::unordered_map<std::string, Variable> table;

public:
    SymbolTable() = default;

    // Declara y valida los límites del tipo de dato en la tabla de símbolos
    bool declareVariable(const Instruction& inst, std::string& outError);
    
    // Consulta si existe una variable
    bool exists(const std::string& name) const;

    bool assignVariable(const Instruction& inst, std::string& outError);
};

} // namespace AphelG

#endif // APHELG_SYMBOL_TABLE_H