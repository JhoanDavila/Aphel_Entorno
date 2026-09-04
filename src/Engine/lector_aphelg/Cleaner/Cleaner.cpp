#include "Cleaner.h"

std::string AphlgCleaner::clean(const std::string& source) {
    std::string result;
    size_t length = source.length();
    size_t i = 0;

    bool inSingleLineComment = false;
    bool inMultiLineComment = false;

    while (i < length) {
        // 1. Manejo de fin de comentario de una línea
        if (inSingleLineComment) {
            if (source[i] == '\n') {
                inSingleLineComment = false;
                result += '\n'; // Preservamos el salto de línea para el conteo del parser
            }
            i++;
            continue;
        }

        // 2. Manejo de cierre de comentario multilínea (\\\)
        if (inMultiLineComment) {
            if (i + 2 < length && source[i] == '\\' && source[i + 1] == '\\' && source[i + 2] == '\\') {
                inMultiLineComment = false;
                i += 3; // Omitir el '\\\'
            } else {
                if (source[i] == '\n') {
                    result += '\n'; // Mantener saltos de línea dentro del comentario para rastrear errores
                }
                i++;
            }
            continue;
        }

        // 3. Detección de inicio de comentario multilínea (///)
        if (i + 2 < length && source[i] == '/' && source[i + 1] == '/' && source[i + 2] == '/') {
            inMultiLineComment = true;
            i += 3;
            continue;
        }

        // 4. Detección de inicio de comentario de una línea (//)
        if (i + 1 < length && source[i] == '/' && source[i + 1] == '/') {
            inSingleLineComment = true;
            i += 2;
            continue;
        }

        // 5. Si no es comentario, conservamos el carácter
        result += source[i];
        i++;
    }

    return result;
}