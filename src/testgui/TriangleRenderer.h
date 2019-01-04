#ifndef TRIANGLERENDERER_H
#define TRIANGLERENDERER_H

#include <array>
#include <SDL.h>
#include <GL/gl.h>

class TriangleRenderer {
public:
    TriangleRenderer(SDL_Window* window, SDL_GLContext context);
    void renderTriangle(std::array<GLfloat, 9> points, std::array<GLfloat, 3> color);

public:
    SDL_Window* window;
    SDL_GLContext context;

    GLuint vertexArrayID;
    GLuint vertexBuffer;
    GLuint colorBuffer;
    GLuint programID;
} ;

#endif // TRIANGLERENDERER_H
