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
private:
    static Vector2D parseVector2D(const std::string& valStr);
    static ColorRGB parseColorRGB(const std::string& valStr);

public:
    static std::shared_ptr<Node> parse(const std::vector<Token>& tokens);
};

#endif // PARSER_H