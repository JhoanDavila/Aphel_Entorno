#ifndef APHELG_APHEL_ENGINE_H
#define APHELG_APHEL_ENGINE_H

#include "../Parser/AST.h"
#include "../../../ui/nodes/node/Node.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace AphelG {

class AphelEngine {
private:
    // Mapea el alias (ej: "ventana") con el nodo raíz de la UI cargada
    std::unordered_map<std::string, std::shared_ptr<Node>> loadedWindows;

    void executeFileDeclaration(const Instruction& inst);
    void executeRenderCommand(const Instruction& inst);

public:
    AphelEngine() = default;

    // Ejecuta la lista de instrucciones del AST
    void execute(const std::vector<Instruction>& instructions);

    // Método de alto nivel que encapsula todo el pipeline (Clean -> Tokenize -> Parse -> Execute)
    static bool runScript(const std::string& filePath);
};

} // namespace AphelG

#endif // APHELG_APHEL_ENGINE_H