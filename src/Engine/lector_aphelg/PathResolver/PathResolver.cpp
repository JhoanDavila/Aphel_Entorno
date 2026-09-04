#include "PathResolver.h"
#include <filesystem>
#include <algorithm>

namespace AphelG {

std::string PathResolver::resolve(const std::string& customPath, const std::string& basePath) {
    std::string path = customPath;
    
    // 1. Reemplazar la sintaxis '--' por '..'
    size_t pos = 0;
    while ((pos = path.find("--", pos)) != std::string::npos) {
        path.replace(pos, 2, "..");
        pos += 2;
    }

    // 2. Reemplazar separadores ';' por '/'
    std::replace(path.begin(), path.end(), ';', '/');

    // 3. Resolver la ruta absoluta
    std::filesystem::path fullPath = path;
    if (!basePath.empty()) {
        fullPath = std::filesystem::path(basePath).parent_path() / path;
    } else {
        // Si no hay basePath, resolver desde el CWD o relativo al directorio del ejecutable
        fullPath = std::filesystem::absolute(path);
    }

    std::error_code ec;
    auto canonicalPath = std::filesystem::weakly_canonical(fullPath, ec);
    
    return (!ec) ? canonicalPath.string() : fullPath.string();
}

} // namespace AphelG