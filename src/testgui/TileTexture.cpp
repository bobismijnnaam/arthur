#include <iostream>
#include <GL/gl3w.h>

#include "TileTexture.h"

TileTexture::TileTexture(int w, int h, SDL_Window* window, SDL_GLContext context, TriangleRenderer& triangleRenderer) : window{window}, context{context}, triangleRenderer{triangleRenderer}, w{w}, h{h}, cellSide{cellSide} {
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
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

    // Always check that our framebuffer is ok
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "SOMEHTING WENT WRONG!\n";
        std::cout << "Code: " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << "\n";
    }
}

void TileTexture::setTextureAsCurrent() {
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glViewport(0, 0, w, h); 
}

void TileTexture::updateState(Tile const & tile) {
    setContextIfNeeded(window, context);
    setTextureAsCurrent();

    renderTile(
            {0, -0.8},
            {0, 0},
            2,
            tile,
            triangleRenderer
            );
}

ImTextureID TileTexture::getTexture() {
    return (ImTextureID)(intptr_t)texID;
}

Vec2i TileTexture::getSize() {
    return Vec2i(w, h);
}

