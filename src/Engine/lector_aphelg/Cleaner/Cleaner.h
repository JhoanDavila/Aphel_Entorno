#ifndef APHLG_CLEANER_H
#define APHLG_CLEANER_H

#include <string>

class AphlgCleaner {
public:
    // Limpia comentarios de una línea (//) y multilínea (/// ... \\\)
    static std::string clean(const std::string& source);
};

#endif // APHLG_CLEANER_H