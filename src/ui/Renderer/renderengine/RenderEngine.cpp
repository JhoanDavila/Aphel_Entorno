#include "RenderEngine.h"

bool RenderEngine::init(const std::shared_ptr<Node>& rootNode) {
    if (!rootNode) {
        return false;
    }

    auto windowNode = std::dynamic_pointer_cast<WindowNode>(rootNode);
    if (!windowNode) {
        return false;
    }

    if (!appWindow.init(windowNode)) {
        return false;
    }

    return true;
}

void RenderEngine::run(const std::shared_ptr<Node>& rootNode) {
    appWindow.startLoop(rootNode);
}

void RenderEngine::stop() {
    appWindow.close();
}