#pragma once

#include <GL/glew.h>
#include <iostream>
#include <string>
#include "../structurerender/StructureRender.h"

class PaintRender {
private:
    GLuint vao = 0; // Vertex Array Object
    GLuint vbo = 0; // Vertex Buffer Object
    GLuint ebo = 0; // Element Buffer Object
    GLuint shaderProgram = 0;
    
    GLint projMatrixLocation = -1;

    // Métodos internos de depuración y compilación
    bool compileShaders();
    void checkShaderErrors(GLuint shader, const std::string& type);
    void checkGLError(const std::string& location) const;

public:
    PaintRender() = default;
    ~PaintRender();

    // Se ejecutará cuando haya un contexto de OpenGL activo
    bool init();
    void setOrthographicProjection(float width, float height);
    void render(const RenderBatch& batch);
    void cleanup();

    // Método para inspeccionar qué se le envía a la GPU por consola
    void debugPrintGPUState(const RenderBatch& batch) const;
};