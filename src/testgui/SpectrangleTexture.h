#ifndef SPECTRANGLETEXTURE_H
#define SPECTRANGLETEXTURE_H

#include <SDL.h>

#include "imgui.h"
#include "TriangleRenderer.h"
#include "Spectrangle.h"

class SpectrangleTexture {
public:
    SpectrangleTexture(int w, int h, float cellSide, SDL_Window* window, SDL_GLContext context, TriangleRenderer& triangleRenderer);

    void updateState(TileBoard const & board);

    ImTextureID getTexture();
    Vec2i getSize();

private:
    void setTextureAsCurrent();
    
    int w;
    int h;
    float cellSide;

    SDL_Window* window;
    SDL_GLContext context;
    TriangleRenderer& triangleRenderer;

    GLuint texID;
    GLuint frameBuffer;
} ;

#endif // SPECTRANGLETEXTURE_H
