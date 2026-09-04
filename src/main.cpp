#include "Engine/lector_aphelg/AphelEngine/AphelEngine.h"

int main() {
    std::string filePath = "/home/jhoan/personal_deloved/Aphel_Entorno/src/Engine/Aphel_Language/example.aphlg";

    // El motor se encarga internamente de todo el pipeline y reporte de errores
    if (!AphelG::AphelEngine::runScript(filePath)) {
        return 1;
    }

    return 0;
}