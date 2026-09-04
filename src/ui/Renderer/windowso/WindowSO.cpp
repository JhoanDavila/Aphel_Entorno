#include "WindowSO.h"

WindowSO::WindowSO() : window(nullptr) {}

WindowSO::~WindowSO() {
    close();
}

bool WindowSO::init(const std::shared_ptr<WindowNode>& windowNode) {
    if (!windowNode) return false;

    if (!glfwInit()) return false;

    int width = windowNode->size.width > 0 ? windowNode->size.width : 800;
    int height = windowNode->size.height > 0 ? windowNode->size.height : 600;
    std::string title = !windowNode->title.empty() ? windowNode->title : "AphelUI Window";

    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!window) {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);

    if (glewInit() != GLEW_OK) return false;

    if (!painter.init()) return false;

    painter.setOrthographicProjection(static_cast<float>(width), static_cast<float>(height));

    return true;
}

void WindowSO::startLoop(const std::shared_ptr<Node>& rootNode) {
    if (!window) return;

    auto winNode = std::dynamic_pointer_cast<WindowNode>(rootNode);

    while (!glfwWindowShouldClose(window)) {
        if (winNode) {
            glClearColor(
                winNode->backgroundColor.rFloat(),
                winNode->backgroundColor.gFloat(),
                winNode->backgroundColor.bFloat(),
                winNode->backgroundColor.aFloat()
            );
        } else {
            glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        }
        glClear(GL_COLOR_BUFFER_BIT);

        RenderBatch batch = structure.buildGeometry(rootNode);
        painter.render(batch);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void WindowSO::close() {
    painter.cleanup();
    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    glfwTerminate();
}