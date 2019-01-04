// dear imgui: standalone example application for SDL2 + OpenGL
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)
// (GL3W is a helper library to access OpenGL functions since there is no standard header to access modern OpenGL functions easily. Alternatives are GLEW, Glad, etc.)

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL.h>
#include <string>
#include <vector>
#include <iostream>
#include <cmath>

// About OpenGL function loaders: modern OpenGL doesn't have a standard header file and requires individual function pointers to be loaded manually. 
// Helper libraries are often used for this purpose! Here we are supporting a few common ones: gl3w, glew, glad.
// You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>    // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>    // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>  // Initialize with gladLoadGL()
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

#include "Vec2i.h"
#include "Vec2f.h"
#include "Spectrangle.h"

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

std::array<std::array<GLfloat, 3>, 6> tileColorToGLColor {{
    {1, 0, 0},
    {0, 0, 1},
    {0, 1, 0},
    {1, 1, 0},
    {0.5, 0, 0.5},
    {1, 1, 1}
}};

float constexpr sqrt_3 = std::sqrt(3);
constexpr float equilateralTriangleHeight(float side) {
    return 0.5 * sqrt_3 * side;
}

// Origin is the position of the middle of the bottom side of the (0, 0) triangle
Vec2f calculateTriangleGridPos(float cellSide, Vec2f origin, Vec2i cell) {
    Vec2f pos;
    pos.y = -cell.y * equilateralTriangleHeight(cellSide);

    pos.x = cell.x * (cellSide * 0.5) - cell.y * (cellSide * 0.5);

    return origin + pos;
}

int msbIndex(int v) {
    if (v == 0) return -1;

    unsigned r = 0;

    while (v >>= 1) {
        r++;
    }

    return r;
}

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

void renderTile(Vec2f origin, Vec2i cell, float cellSide, Tile tile, TriangleRenderer& triangleRenderer) {
    float height = equilateralTriangleHeight(cellSide);

    Vec2f base = calculateTriangleGridPos(cellSide, origin, cell);
    Vec2f verticalPoint;
    Vec2f leftPoint;
    Vec2f rightPoint;

    bool isUpTile = cell.x % 2 == 0;
    if (isUpTile) {
        verticalPoint = base + Vec2f(0, height);
        leftPoint = base + Vec2f(-cellSide / 2, 0);
        rightPoint = base + Vec2f(cellSide / 2, 0);
    } else {
        verticalPoint = base;
        leftPoint = base + Vec2f(-cellSide / 2, height);
        rightPoint = base + Vec2f(cellSide / 2, height);
    }

    Vec2f centerPoint(base.x, (verticalPoint.y + leftPoint.y + rightPoint.y) / 3.0);
    
    // Right side
    int colorIndex = msbIndex((int) tile.sides[0]);
    triangleRenderer.renderTriangle({
            centerPoint.x, centerPoint.y, 0,
            verticalPoint.x, verticalPoint.y, 0,
            rightPoint.x, rightPoint.y, 0
    }, tileColorToGLColor[colorIndex]);

    // top/bottom side
    colorIndex = msbIndex((int) tile.sides[1]);
    triangleRenderer.renderTriangle({
        leftPoint.x, leftPoint.y, 0,
        rightPoint.x, rightPoint.y, 0,
        centerPoint.x, centerPoint.y, 0
    }, tileColorToGLColor[colorIndex]);

    // Left side
    colorIndex = msbIndex((int) tile.sides[2]);
    triangleRenderer.renderTriangle({
        leftPoint.x, leftPoint.y, 0,
        verticalPoint.x, verticalPoint.y, 0,
        centerPoint.x, centerPoint.y, 0
    }, tileColorToGLColor[colorIndex]);
}

#include <optional>
#include "Spectrangle.h"

class SpectrangleTexture {
public:
    SpectrangleTexture(int w, int h, float cellSide, SDL_Window* window, SDL_GLContext context, TriangleRenderer& triangleRenderer);

    void updateState(TileBoard const & board);

    ImTextureID getTexture();
    Vec2i getSize();

private:
    void setTextureAsCurrent();

    TriangleRenderer& triangleRenderer;

    SDL_Window* window;
    SDL_GLContext context;

    GLuint texID;
    GLuint frameBuffer;

    int w;
    int h;
    float cellSide;
} ;

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
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
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

int main(int, char**)
{
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // Decide GL+GLSL versions
#if __APPLE__
    // GL 3.2 Core + GLSL 150
    const char* glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);

    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);
    SDL_Window* window = SDL_CreateWindow("Dear ImGui SDL2+OpenGL3 example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = gladLoadGL() == 0;
#else
    bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

    // Setup Platform/Renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Setup Style
    ImGui::StyleColorsDark();

    SDL_GLContext scratchContext = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, scratchContext);

    TriangleRenderer triangleRenderer(window, scratchContext);

    TileTexture tile1(100, 100, window, scratchContext, triangleRenderer);
    tile1.updateState({Color::RED, Color::RED, Color::RED});
    
    SpectrangleTexture spectrangleTexture(500, 500, 0.18, window, scratchContext, triangleRenderer);
    TileBoard board;
    board.set({0, 0}, {{Color::RED, Color::RED, Color::RED}});
    board.set({0, 1}, {{Color::BLUE, Color::BLUE, Color::PURPLE}});
    board.set({0, 2}, {{Color::GREEN, Color::GREEN, Color::GREEN}});
    board.set({0, 3}, {{Color::YELLOW, Color::GREEN, Color::YELLOW}});
    board.set({0, 4}, {{Color::PURPLE, Color::PURPLE, Color::PURPLE}});
    board.set({0, 5}, {{Color::WHITE, Color::WHITE, Color::WHITE}});
    spectrangleTexture.updateState(board);

    // Main starts here
    bool show_demo_window = true;
    bool show_another_window = false;
    // ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImVec4 clear_color = ImVec4(0, 0, 0, 1.00f);

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            ImGui::Begin("Spectrangle game state");                          

            ImVec2 windowSize = ImGui::GetWindowSize();
            ImVec2 framePadding(10, 10);

            ImGui::Image(spectrangleTexture.getTexture(), ImVec2(windowSize.x - framePadding.x * 2, windowSize.x - framePadding.x * 2), ImVec2(0,1), ImVec2(1,0), ImColor(255,255,255,255), ImColor(255,255,255,128));

            ImGui::ImageButton(tile1.getTexture(), ImVec2(100,100), ImVec2(0,1), ImVec2(1, 0), 1, ImColor(0,0,0,255));

            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state

            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        SDL_GL_MakeCurrent(window, gl_context);
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
