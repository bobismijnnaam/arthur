#ifndef GUIUTILS_H
#define GUIUTILS_H

#include <cmath>

#include "TriangleRenderer.h"
#include "Move.h"
#include "Vec2f.h"

float constexpr sqrt_3 = std::sqrt(3);

constexpr float equilateralTriangleHeight(float side) {
    return 0.5 * sqrt_3 * side;
}

int msbIndex(int v);

Vec2f calculateTriangleGridPos(float cellSide, Vec2f origin, Vec2i cell);

void renderTile(Vec2f origin, Vec2i cell, float cellSide, Tile tile, TriangleRenderer& triangleRenderer);

void setContextIfNeeded(SDL_Window* window, SDL_GLContext context);

GLuint loadShadersFromString(std::string vertex_shader, std::string fragment_shader);

#endif // GUIUTILS_H
