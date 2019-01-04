#ifndef TILETEXTURE_H
#define TILETEXTURE_H

#include <SDL.h>

#include "imgui.h"
#include "TriangleRenderer.h"
#include "GuiUtils.h"

class TileTexture {
public:
    TileTexture(int w, int h, SDL_Window* window, SDL_GLContext context, TriangleRenderer& triangleRenderer);

    void updateState(Tile const & tile);

    ImTextureID getTexture();
    Vec2i getSize();

private:
    void setTextureAsCurrent();
    
    SDL_Window* window;
    SDL_GLContext context;
    TriangleRenderer& triangleRenderer;

    GLuint texID;
    GLuint frameBuffer;

    int w;
    int h;
    float cellSide;
} ;

#endif // TILETEXTURE_H
