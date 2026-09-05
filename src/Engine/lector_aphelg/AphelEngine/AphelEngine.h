#ifndef APHEL_ENGINE_H
#define APHEL_ENGINE_H

#include "../Parser/AST.h"
#include "../SymbolTable/SymbolTable.h"
#include "../../../ui/lector_aphelui/reader/AphluiReader.h"
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

namespace AphelG {

class AphelEngine {
public:
    static bool runScript(const std::string& filePath);
    void execute(const std::vector<Instruction>& instructions);

private:
    SymbolTable symbolTable; // Agregado para resolver 'symbolTable is undefined'
    std::unordered_map<std::string, std::shared_ptr<Node>> loadedWindows;

    void executeFileDeclaration(const Instruction& inst);
    void executeRenderCommand(const Instruction& inst);
    void executeDataDeclaration(const Instruction& inst);
    void executeVariableAssignment(const Instruction& inst); // Agregado para resolver 'executeVariableAssignment is undefined'
};

} // namespace AphelG

#endif // APHEL_ENGINE_H