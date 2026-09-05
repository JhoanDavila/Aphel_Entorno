#include "SymbolTable.h"
#include <limits>
#include <cstdint>

namespace AphelG {

bool SymbolTable::exists(const std::string& name) const {
    return table.find(name) != table.end();
}

bool SymbolTable::declareVariable(const Instruction& inst, std::string& outError) {
    if (exists(inst.varName)) {
        outError = "Redeclaración de variable: '" + inst.varName + "' ya existe en el ámbito actual.";
        return false;
    }

    Variable var;
    var.name = inst.varName;
    var.type = inst.dataType;

    switch (inst.dataType) {
        case DataType::TXT: {
            std::string cleanValue = inst.rawValue;

            if (cleanValue.length() >= 2 && cleanValue.front() == '"' && cleanValue.back() == '"') {
                cleanValue = cleanValue.substr(1, cleanValue.length() - 2);
            }

            var.stringVal = cleanValue;
            break;
        }

        case DataType::INT: {
            if (inst.rawValue.find('.') != std::string::npos) {
                outError = "Error de Tipo: Se intentó asignar un decimal '" + inst.rawValue + "' a un 'int'.";
                return false;
            }
            try {
                long long val = std::stoll(inst.rawValue);
                
                // Validación contra el rango estándar de int32_t (-2,147,483,648 a 2,147,483,647)
                if (val > std::numeric_limits<int32_t>::max() || val < std::numeric_limits<int32_t>::min()) {
                    outError = "Desbordamiento de rango para 'int': " + inst.rawValue + 
                               " excede el rango estándar de entero de 32 bits.";
                    return false;
                }
                var.intVal = static_cast<int32_t>(val);
            } catch (...) {
                outError = "Error de formato o desbordamiento: El valor '" + inst.rawValue + "' no es un 'int' válido.";
                return false;
            }
            break;
        }

        case DataType::NAT: {
            if (inst.rawValue.find('.') != std::string::npos) {
                outError = "Error de Tipo: Se intentó asignar un decimal '" + inst.rawValue + "' a un 'nat'.";
                return false;
            }
            try {
                if (!inst.rawValue.empty() && inst.rawValue.front() == '-') {
                    outError = "Violación de tipo 'nat': Solo se aceptan números naturales (>= 0). Valor dado: " + inst.rawValue;
                    return false;
                }

                unsigned long long val = std::stoull(inst.rawValue);
                
                // Validación contra el rango estándar de uint32_t (0 a 4,294,967,295)
                if (val > std::numeric_limits<uint32_t>::max()) {
                    outError = "Desbordamiento de rango para 'nat': El valor " + inst.rawValue + 
                               " excede el máximo permitido para un número natural estándar.";
                    return false;
                }
                var.natVal = static_cast<uint32_t>(val);
            } catch (...) {
                outError = "Error de formato o desbordamiento: El valor '" + inst.rawValue + "' no es un 'nat' válido.";
                return false;
            }
            break;
        }

        case DataType::DBL: {
            try {
                var.dblVal = std::stod(inst.rawValue);
            } catch (...) {
                outError = "Error de formato: El valor '" + inst.rawValue + "' no es un decimal (dbl) válido.";
                return false;
            }
            break;
        }

        case DataType::BOOL: {
            if (inst.rawValue == "true" || inst.rawValue == "1") {
                var.boolVal = true;
            } else if (inst.rawValue == "false" || inst.rawValue == "0") {
                var.boolVal = false;
            } else {
                outError = "Violación de tipo 'bool': Se esperaba 'true', 'false', '1' o '0', se recibió '" + inst.rawValue + "'.";
                return false;
            }
            break;
        }

        default:
            outError = "Tipo de dato desconocido para la variable '" + inst.varName + "'.";
            return false;
    }

    table[inst.varName] = var;
    return true;
}

bool SymbolTable::assignVariable(const Instruction& inst, std::string& outError) {
    auto it = table.find(inst.varName);
    if (it == table.end()) {
        outError = "Error Semántico: La variable '" + inst.varName + "' no ha sido declarada previamente.";
        return false;
    }

    Variable& var = it->second;

    // Si la instrucción especifica un nuevo tipo (distinto de NONE), se actualiza el tipo
    if (inst.dataType != DataType::NONE) {
        var.type = inst.dataType;
    }

    // Validar y almacenar el nuevo valor según el tipo objetivo final
    switch (var.type) {
        case DataType::TXT: {
            std::string cleanValue = inst.rawValue;
            if (cleanValue.length() >= 2 && cleanValue.front() == '"' && cleanValue.back() == '"') {
                cleanValue = cleanValue.substr(1, cleanValue.length() - 2);
            } else if (exists(cleanValue) && table[cleanValue].type == DataType::TXT) {
                cleanValue = table[cleanValue].stringVal;
            } else {
                outError = "Error de Tipo: No se puede asignar '" + inst.rawValue + "' a la variable 'txt' '" + inst.varName + "'. Se esperaba una cadena de texto entre comillas.";
                return false;
            }
            var.stringVal = cleanValue;
            break;
        }
        case DataType::INT: {
            if (inst.rawValue.find('.') != std::string::npos) {
                outError = "Error de Tipo: No se puede asignar un decimal '" + inst.rawValue + "' a la variable 'int' '" + inst.varName + "'.";
                return false;
            }
            try {
                long long val = std::stoll(inst.rawValue);
                if (val > std::numeric_limits<int32_t>::max() || val < std::numeric_limits<int32_t>::min()) {
                    outError = "Desbordamiento para 'int' en reasignación: " + inst.rawValue;
                    return false;
                }
                var.intVal = static_cast<int32_t>(val);
            } catch (...) {
                outError = "Error de Tipo: No se puede asignar '" + inst.rawValue + "' a la variable 'int' '" + inst.varName + "'.";
                return false;
            }
            break;
        }
        case DataType::NAT: {
            if (inst.rawValue.find('.') != std::string::npos) {
                outError = "Error de Tipo: No se puede asignar un decimal '" + inst.rawValue + "' a la variable 'nat' '" + inst.varName + "'.";
                return false;
            }
            try {
                if (!inst.rawValue.empty() && inst.rawValue.front() == '-') {
                    outError = "Violación de tipo 'nat': No se admiten negativos.";
                    return false;
                }
                unsigned long long val = std::stoull(inst.rawValue);
                if (val > std::numeric_limits<uint32_t>::max()) {
                    outError = "Desbordamiento para 'nat' en reasignación: " + inst.rawValue;
                    return false;
                }
                var.natVal = static_cast<uint32_t>(val);
            } catch (...) {
                outError = "Error de Tipo: No se puede asignar '" + inst.rawValue + "' a la variable 'nat' '" + inst.varName + "'.";
                return false;
            }
            break;
        }
        case DataType::DBL: {
            try {
                var.dblVal = std::stod(inst.rawValue);
            } catch (...) {
                outError = "Error de Tipo: No se puede asignar '" + inst.rawValue + "' a la variable 'dbl' '" + inst.varName + "'.";
                return false;
            }
            break;
        }
        case DataType::BOOL: {
            if (inst.rawValue == "true" || inst.rawValue == "1") var.boolVal = true;
            else if (inst.rawValue == "false" || inst.rawValue == "0") var.boolVal = false;
            else {
                outError = "Error de Tipo: No se puede asignar '" + inst.rawValue + "' a la variable 'bool' '" + inst.varName + "'.";
                return false;
            }
            break;
        }
        default:
            outError = "Tipo no reconocido para la reasignación.";
            return false;
    }

    return true;
}

} // namespace AphelG