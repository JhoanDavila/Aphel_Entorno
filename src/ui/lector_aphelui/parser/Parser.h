#ifndef PARSER_H
#define PARSER_H

#include "ui/lector_aphelui/tokenizer/Token.h"
#include "ui/nodes/node/Node.h"
#include "ui/nodes/visual_node/VisualNode.h"
#include "ui/nodes/2dspace/2DSpace.h"
#include "ui/nodes/window/Window.h"

#include <vector>
#include <memory>
#include <string>

class Parser {
public:
    // Métodos auxiliares hechos públicos para funciones internas del archivo .cpp
    static bool tryParseVector2D(const std::string& valStr, Vector2D& outVec);
    static bool tryParseColor(const std::string& valStr, Color& outColor, int line);
    static bool isValidStringFormat(const std::string& valStr);

    static std::shared_ptr<Node> parse(const std::vector<Token>& tokens);
};

#endif // PARSER_H