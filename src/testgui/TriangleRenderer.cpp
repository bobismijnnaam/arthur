#include <GL/gl3w.h>

#include "TriangleRenderer.h"
#include "GuiUtils.h"

TriangleRenderer::TriangleRenderer(SDL_Window* window, SDL_GLContext context) : window{window}, context{context} {
    setContextIfNeeded(window, context);

    glGenVertexArrays(1, &vertexArrayID);
    glGenBuffers(1, &vertexBuffer);
    glGenBuffers(1, &colorBuffer);

    std::string vertexShader = R"(
#version 330 core
// #version 130
layout(location = 0) in vec3 vertexPosition_modelspace;
// Notice that the "1" here equals the "1" in glVertexAttribPointer
layout(location = 1) in vec3 vertexColor;

out vec3 fragmentColor;

void main(){
    gl_Position.xyz = vertexPosition_modelspace;
    gl_Position.w = 1.0;

    fragmentColor = vertexColor;
}
        )";

    std::string fragmentShader = R"(
#version 330 core
// #version 130
// Interpolated values from the vertex shaders
in vec3 fragmentColor;
out vec4 color;
void main(){
    color.xyz = fragmentColor;
    color.w = 1;
}
        )";

    programID = loadShadersFromString(vertexShader, fragmentShader);
}

void TriangleRenderer::renderTriangle(std::array<GLfloat, 9> points, std::array<GLfloat, 3> color) {
    GLint drawFboId = 0;
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFboId);

    setContextIfNeeded(window, context);

    GLfloat g_color_buffer_data[] = {
        color[0], color[1], color[2],
        color[0], color[1], color[2],
        color[0], color[1], color[2]
    };

    // Draw triangle
    glBindVertexArray(vertexArrayID);

    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points.data(), GL_STATIC_DRAW);

    // Give our color data to OpenGL
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

    // 1st attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(
       0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
       3,                  // size
       GL_FLOAT,           // type
       GL_FALSE,           // normalized?
       0,                  // stride
       (void*)0            // array buffer offset
    );

    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glVertexAttribPointer(
        1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    // Draw the triangle !
    glUseProgram(programID);

    glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

