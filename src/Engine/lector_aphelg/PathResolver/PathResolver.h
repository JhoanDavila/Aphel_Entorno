#ifndef APHELG_PATH_RESOLVER_H
#define APHELG_PATH_RESOLVER_H

#include <string>
#include <filesystem>

namespace AphelG {

class PathResolver {
public:
    // Convierte la sintaxis custom de AphelG a una ruta de sistema operativo
    static std::string resolve(const std::string& customPath, const std::string& basePath = "");
};

} // namespace AphelG

#endif // APHELG_PATH_RESOLVER_H