#include <vector>
#include <GL/gl3w.h>

#include "Vec2i.h"
#include "GuiUtils.h"

int msbIndex(int v) {
    if (v == 0) return -1;

    unsigned r = 0;

    while (v >>= 1) {
        r++;
    }

    return r;
}

// Origin is the position of the middle of the bottom side of the (0, 0) triangle
Vec2f calculateTriangleGridPos(float cellSide, Vec2f origin, Vec2i cell) {
    Vec2f pos;
    pos.y = -cell.y * equilateralTriangleHeight(cellSide);

    pos.x = cell.x * (cellSide * 0.5) - cell.y * (cellSide * 0.5);

    return origin + pos;
}

std::array<std::array<GLfloat, 3>, 6> const tileColorToGLColor {{
    {1, 0, 0},
    {0, 0, 1},
    {0, 1, 0},
    {1, 1, 0},
    {0.5, 0, 0.5},
    {1, 1, 1}
}};

void renderTile(Vec2f origin, Vec2i cell, float cellSide, Tile tile, TriangleRenderer& triangleRenderer) {
    float height = equilateralTriangleHeight(cellSide);

    Vec2f base = calculateTriangleGridPos(cellSide, origin, cell);
    std::array<Vec2f, 3> points;

    bool isUpTile = cell.x % 2 == 0;
    if (isUpTile) {
        points[0] = base + Vec2f(0, height);
        points[1] = base + Vec2f(cellSide / 2, 0);
        points[2] = base + Vec2f(-cellSide / 2, 0);
    } else {
        points[0] = base + Vec2f(cellSide / 2, height);
        points[1] = base;
        points[2] = base + Vec2f(-cellSide / 2, height);
    }

    Vec2f centerPoint(base.x, (points[0].y + points[1].y + points[2].y) / 3.0);
    
    // Right side
    int colorIndex = msbIndex((int) tile.sides[0]);
    triangleRenderer.renderTriangle({
            centerPoint.x, centerPoint.y, 0,
            points[0].x, points[0].y, 0,
            points[1].x, points[1].y, 0
    }, tileColorToGLColor[colorIndex]);

    // top/bottom side
    colorIndex = msbIndex((int) tile.sides[1]);
    triangleRenderer.renderTriangle({
        centerPoint.x, centerPoint.y, 0,
        points[1].x, points[1].y, 0,
        points[2].x, points[2].y, 0,
    }, tileColorToGLColor[colorIndex]);

    // Left side
    colorIndex = msbIndex((int) tile.sides[2]);
    triangleRenderer.renderTriangle({
        centerPoint.x, centerPoint.y, 0,
        points[2].x, points[2].y, 0,
        points[0].x, points[0].y, 0,
    }, tileColorToGLColor[colorIndex]);
}

void setContextIfNeeded(SDL_Window* window, SDL_GLContext context) {
    SDL_GLContext current = SDL_GL_GetCurrentContext();
    if (current != context) {
        SDL_GL_MakeCurrent(window, context);
    }
}

GLuint loadShadersFromString(std::string vertex_shader, std::string fragment_shader){

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode = vertex_shader;

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode = fragment_shader;

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling vertex shader\n");
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling fragment shader\n");
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}
	
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

