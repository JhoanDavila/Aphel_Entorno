#include "PaintRender.h"

// Shaders embebidos para dibujar en 2D (Posición + Color)
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;
    layout (location = 1) in vec4 aColor;

    out vec4 vertexColor;
    uniform mat4 u_Projection;

    void main() {
        gl_Position = u_Projection * vec4(aPos, 0.0, 1.0);
        vertexColor = aColor;
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    in vec4 vertexColor;
    out vec4 FragColor;

    void main() {
        FragColor = vertexColor;
    }
)";

PaintRender::~PaintRender() {
    cleanup();
}

bool PaintRender::init() {
    std::cout << "[PaintRender Debug] Creando buffers y Shaders..." << std::endl;

    if (!compileShaders()) {
        std::cerr << "[PaintRender Error] Falló la compilación de Shaders." << std::endl;
        return false;
    }

    // Reservar identificadores en la memoria de la GPU
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    // Configuración del VBO (Puntos de los vértices)
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    // Atributo 0: Posición (X, Y)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    // Atributo 1: Color (R, G, B, A)
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBindVertexArray(0);

    checkGLError("PaintRender::init");
    std::cout << "[PaintRender Debug] Buffers y Shaders configurados." << std::endl;
    return true;
}

void PaintRender::setOrthographicProjection(float width, float height) {
    glUseProgram(shaderProgram);

    // Matriz de proyección para trabajar en píxeles (0,0 arriba a la izquierda)
    float left = 0.0f, right = width;
    float top = 0.0f, bottom = height;
    float farVal = 1.0f, nearVal = -1.0f;

    float ortho[16] = {
        2.0f / (right - left), 0.0f,                  0.0f,                       0.0f,
        0.0f,                  2.0f / (top - bottom), 0.0f,                       0.0f,
        0.0f,                  0.0f,                 -2.0f / (farVal - nearVal),  0.0f,
       -(right + left) / (right - left), -(top + bottom) / (top - bottom), -(farVal + nearVal) / (farVal - nearVal), 1.0f
    };

    projMatrixLocation = glGetUniformLocation(shaderProgram, "u_Projection");
    glUniformMatrix4fv(projMatrixLocation, 1, GL_FALSE, ortho);

    checkGLError("setOrthographicProjection");
}

void PaintRender::render(const RenderBatch& batch) {
    if (batch.vertices.empty() || batch.indices.empty()) return;

    glUseProgram(shaderProgram);
    glBindVertexArray(vao);

    // Enviar vértices e índices frescos a la VRAM
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, batch.vertices.size() * sizeof(Vertex), batch.vertices.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, batch.indices.size() * sizeof(unsigned int), batch.indices.data(), GL_DYNAMIC_DRAW);

    // Orden de dibujado
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(batch.indices.size()), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    checkGLError("PaintRender::render");
}

void PaintRender::debugPrintGPUState(const RenderBatch& batch) const {
    std::cout << "\n================ [DEBUG PAINT RENDER] ================" << std::endl;
    std::cout << "  - ID VAO: " << vao << std::endl;
    std::cout << "  - ID VBO: " << vbo << std::endl;
    std::cout << "  - ID EBO: " << ebo << std::endl;
    std::cout << "  - ID Shader Program: " << shaderProgram << std::endl;
    std::cout << "  - Bytes en VBO: " << batch.vertices.size() * sizeof(Vertex) << " bytes" << std::endl;
    std::cout << "  - Bytes en EBO: " << batch.indices.size() * sizeof(unsigned int) << " bytes" << std::endl;
    std::cout << "  - Triángulos a pintar: " << batch.indices.size() / 3 << std::endl;
    std::cout << "======================================================\n" << std::endl;
}

bool PaintRender::compileShaders() {
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShaderSource, NULL);
    glCompileShader(vs);
    checkShaderErrors(vs, "VERTEX");

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShaderSource, NULL);
    glCompileShader(fs);
    checkShaderErrors(fs, "FRAGMENT");

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);
    checkShaderErrors(shaderProgram, "PROGRAM");

    glDeleteShader(vs);
    glDeleteShader(fs);

    return shaderProgram != 0;
}

void PaintRender::checkShaderErrors(GLuint shader, const std::string& type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "[Shader Error - " << type << "]: " << infoLog << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "[Shader Link Error]: " << infoLog << std::endl;
        }
    }
}

void PaintRender::checkGLError(const std::string& location) const {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "[OpenGL Error] (" << location << "): 0x" << std::hex << err << std::dec << std::endl;
    }
}

void PaintRender::cleanup() {
    if (vao) glDeleteVertexArrays(1, &vao);
    if (vbo) glDeleteBuffers(1, &vbo);
    if (ebo) glDeleteBuffers(1, &ebo);
    if (shaderProgram) glDeleteProgram(shaderProgram);
}