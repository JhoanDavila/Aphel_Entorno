#include "AphelEngine.h"
#include "../Cleaner/Cleaner.h"
#include "../Tokenizer/Tokenizer.h"
#include "../Parser/Parser.h"
#include "../PathResolver/PathResolver.h"
#include "../ErrorManager/ErrorManager.h"
#include "../../../ui/lector_aphelui/reader/AphluiReader.h"
#include "../../../ui/Renderer/renderengine/RenderEngine.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace AphelG {

bool AphelEngine::runScript(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        ErrorManager::logError(0, "Error: No se pudo abrir el archivo " + filePath);
        return false;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    // 1. Limpieza de comentarios
    std::string cleanedScript = AphlgCleaner::clean(buffer.str());

    // 2. Tokenización
    Tokenizer tokenizer(cleanedScript);
    std::vector<Token> tokens = tokenizer.tokenize();

    // 3. Análisis Sintáctico (Parser)
    Parser parser(tokens);
    std::vector<Instruction> ast = parser.parse();

    // Validar errores estáticos (sintaxis y tokenización)
    if (ErrorManager::hasErrors()) {
        std::cout << "--- ERRORES DE COMPILACIÓN/SINTAXIS ---" << std::endl;
        ErrorManager::printErrors();
        return false;
    }

    // 4. Ejecución del runtime
    std::cout << "--- EJECUTANDO SCRIPT APHLG ---" << std::endl;
    AphelEngine engine;
    engine.execute(ast);

    // Validar errores de ejecución (rutas no encontradas, aliases inválidos)
    if (ErrorManager::hasErrors()) {
        std::cout << "--- ERRORES EN TIEMPO DE EJECUCIÓN ---" << std::endl;
        ErrorManager::printErrors();
        return false;
    }

    return true;
}

void AphelEngine::executeFileDeclaration(const Instruction& inst) {
    // Resuelve la ruta utilizando la ruta base del script para mayor precisión
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