#include "AphelEngine.h"
#include "../PathResolver/PathResolver.h"
#include "../ErrorManager/ErrorManager.h"
#include "../../../ui/lector_aphelui/reader/AphluiReader.h"
#include "../../../ui/Renderer/renderengine/RenderEngine.h"

namespace AphelG {

void AphelEngine::executeFileDeclaration(const Instruction& inst) {
    std::string realPath = PathResolver::resolve(inst.path);

    // Carga el árbol de nodos del archivo .aphlui
    std::shared_ptr<Node> uiRoot = AphluiReader::loadFromFile(realPath);

    if (!uiRoot) {
        ErrorManager::logError(inst.line, "Error de Ejecución: No se pudo cargar la UI en '" + realPath + "'");
        return;
    }

    loadedWindows[inst.alias] = uiRoot;
}

void AphelEngine::executeRenderCommand(const Instruction& inst) {
    auto it = loadedWindows.find(inst.alias);
    if (it == loadedWindows.end()) {
        ErrorManager::logError(inst.line, "Error Semántico: El recurso '" + inst.alias + "' no ha sido declarado con 'File'.");
        return;
    }

    // Instanciación e invocación del RenderEngine
    RenderEngine renderer;
    if (!renderer.init(it->second)) {
        ErrorManager::logError(inst.line, "Error de Renderizado: No se pudo inicializar la ventana para '" + inst.alias + "'");
        return;
    }

    // Inicia el bucle de renderizado (Game/UI Loop)
    renderer.run(it->second);
}

void AphelEngine::execute(const std::vector<Instruction>& instructions) {
    for (const auto& inst : instructions) {
        if (inst.type == InstructionType::FILE_DECLARATION) {
            executeFileDeclaration(inst);
        }
        else if (inst.type == InstructionType::RENDER_COMMAND) {
            executeRenderCommand(inst);
        }
    }
}

} // namespace AphelG