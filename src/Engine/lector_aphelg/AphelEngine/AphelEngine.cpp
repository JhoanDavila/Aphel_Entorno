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

    // Validar errores de ejecución (rutas no encontradas, desbordamientos de memoria, etc.)
    if (ErrorManager::hasErrors()) {
        std::cout << "--- ERRORES EN TIEMPO DE EJECUCIÓN ---" << std::endl;
        ErrorManager::printErrors();
        return false;
    }

    return true;
}

void AphelEngine::executeFileDeclaration(const Instruction& inst) {
    std::string realPath = PathResolver::resolve(inst.path);

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

    RenderEngine renderer;
    if (!renderer.init(it->second)) {
        ErrorManager::logError(inst.line, "Error de Renderizado: No se pudo inicializar la ventana para '" + inst.alias + "'");
        return;
    }

    renderer.run(it->second);
}

void AphelEngine::executeDataDeclaration(const Instruction& inst) {
    std::string errorMessage;
    if (!symbolTable.declareVariable(inst, errorMessage)) {
        ErrorManager::logError(inst.line, errorMessage);
    }
}

// NUEVO: Método para ejecutar reasignación de variables
void AphelEngine::executeVariableAssignment(const Instruction& inst) {
    std::string errorMessage;
    if (!symbolTable.assignVariable(inst, errorMessage)) {
        ErrorManager::logError(inst.line, errorMessage);
    }
}

void AphelEngine::execute(const std::vector<Instruction>& instructions) {
    for (const auto& inst : instructions) {
        if (inst.type == InstructionType::FILE_DECLARATION) {
            executeFileDeclaration(inst);
        }
        else if (inst.type == InstructionType::RENDER_COMMAND) {
            executeRenderCommand(inst);
        }
        else if (inst.type == InstructionType::DATA_DECLARATION) {
            executeDataDeclaration(inst);
        }
        else if (inst.type == InstructionType::VARIABLE_ASSIGNMENT) { // NUEVO: Evaluador de reasignaciones
            executeVariableAssignment(inst);
        }
    }
}

} // namespace AphelG