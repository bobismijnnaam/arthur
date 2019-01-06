#include <GL/gl3w.h>
#include <iostream>

#include "GuiUtils.h"
#include "SpectrangleTexture.h"

SpectrangleTexture::SpectrangleTexture(int w, int h, float cellSide, SDL_Window* window, SDL_GLContext context, TriangleRenderer& triangleRenderer) : w{w}, h{h}, cellSide{cellSide}, window{window}, context{context}, triangleRenderer{triangleRenderer} {
    setContextIfNeeded(window, context);

    // Create custom texture
    // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
    glGenFramebuffers(1, &frameBuffer);
    setTextureAsCurrent();

    // The texture we're going to render to
    glGenTextures(1, &texID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, texID);

    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    // Poor filtering. Needed !
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // The depth buffer
    GLuint depthRenderBuffer;
    glGenRenderbuffers(1, &depthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);

    // Set "renderedTexture" as our colour attachement #0
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texID, 0);

    // Set the list of draw buffers.
    GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

    // Always check that our framebuffer is ok
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "SOMEHTING WENT WRONG!\n";
        std::cout << "Code: " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << "\n";
    }
}

void SpectrangleTexture::setTextureAsCurrent() {
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glViewport(0, 0, w, h); 
}

std::array<GLfloat, 3> red = {1, 0, 0};
std::array<GLfloat, 3> green = {0, 1, 0};

void SpectrangleTexture::updateState(TileBoard const & board) {
    setContextIfNeeded(window, context);
    setTextureAsCurrent();

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    Vec2f origin {0, 0.85f - equilateralTriangleHeight(cellSide)};
    
    for (int y = 0; y < SPECTRANGLE_BOARD_SIDE; ++y) {
        for (int x = 0; x < TileBoard::rowLength(y); ++x) {
            std::optional<Tile> const & tile = board.get({x, y});
            if (tile.has_value()) {
                renderTile(origin, {x, y}, cellSide, *tile, triangleRenderer);
            }
        }
    }
}

ImTextureID SpectrangleTexture::getTexture() {
    return (ImTextureID)(intptr_t)texID;
}

Vec2i SpectrangleTexture::getSize() {
    return {w, h};
}
