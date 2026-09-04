#include "PathResolver.h"
#include <sstream>
#include <vector>

namespace AphelG {

std::string PathResolver::resolve(const std::string& customPath, const std::string& basePath) {
    if (customPath.empty()) return "";

    std::vector<std::string> segments;
    std::stringstream ss(customPath);
    std::string segment;

    // 1. Separar por el caracter ';'
    while (std::getline(ss, segment, ';')) {
        if (!segment.empty()) {
            segments.push_back(segment);
        }
    }

    // 2. Construir la ruta relativa/absoluta
    std::filesystem::path resultPath = basePath;

    for (const auto& seg : segments) {
        if (seg == "--") {
            // "--" equivale a subir un directorio ("..")
            resultPath /= "..";
        } else {
            resultPath /= seg;
        }
    }

    // 3. Devolver la ruta normalizada estilo C++ std::filesystem
    return resultPath.lexically_normal().string();
}

} // namespace AphelG