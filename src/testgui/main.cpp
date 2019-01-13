#include <stdio.h>
#include <SDL.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <cmath>
#include <GL/gl3w.h>
#include <chrono>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include "Spectrangle.h"
#include "SpectrangleGameStateWindow.h"
#include "TileTexture.h"
#include "TriangleRenderer.h"
#include "Vec2f.h"
#include "Vec2i.h"
#include "SpectrangleTexture.h"
#include "RandomAI.h"
#include "StochasticAI.h"

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
    SDL_Window* window = SDL_CreateWindow("Spectrangle Monolithic Player vs. AI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Initialize OpenGL loader
    bool err = gl3wInit() != 0;
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

    SpectrangleGameStateWindow gameStateWindow("Spectrangle Game State Window", window, scratchContext);

    Spectrangle game(2);
    Random random;
    int currentPlayer = 0;
    game.initializePlayerBags(random);

    // Main starts here
    bool show_demo_window = false;

    ImVec4 clear_color = ImVec4(0.3, 0.3, 0.3, 1.00f);

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
            ImGui::Begin("Demo window appendix");                          

            ImGui::Checkbox("Demo Window", &show_demo_window);

            ImGui::End();
        }

        {
            ImGui::Begin("Game controls");

            if (ImGui::Button("Reset game")) {
                game = Spectrangle(2);
                game.initializePlayerBags(random);
                currentPlayer = 0;
            }

            if (ImGui::Button("Random AI move")) {
                std::cout << "Random ai move!\n";
                GameMove gameMove = randomAI(game, currentPlayer, random);
                if (gameMove.moveType == GameMoveType::MOVE) {
                    game.applyMove(currentPlayer, gameMove.move, random);
                } else {
                    std::cout << "Move was: " << (gameMove.moveType == GameMoveType::SKIP ? "SKIP" : "EXCHANGE") << "\n";
                }
                currentPlayer = (currentPlayer + 1) % game.getNumPlayers();
            }

            if (ImGui::Button("Player move")) {
                std::cout << "Unsupported!\n";
                std::exit(1);
            }

            if (ImGui::Button("Print all tiles!")) {
                auto const & board = game.getBoard();
                for (int y = 0; y < SPECTRANGLE_BOARD_SIDE; ++y) {
                    for (int x = 0; x < TileBoard::rowLength(y); ++x) {
                        if (auto tileOpt = board.get({x, y})) {
                            std::cout << *tileOpt << "\n";
                        }
                    }
                }
            }

            ImGui::Text("Current player: %d", currentPlayer);


            ImGui::End();
        }

        {
            ImGui::Begin("Stochastic AI info");

            static bool isCalculating = false;
            static int maxCycles = 1000;
            static int cyclesPerIteration = 50;
            static GameMove gameMove = GameMove::Skip(0);
            ImGui::InputInt("Max cycles", &maxCycles);
            ImGui::InputInt("Cycles per iteration", &cyclesPerIteration);

            static PausableStochasticAI pausableStochasticAI(maxCycles, cyclesPerIteration);
            pausableStochasticAI.setMaxCycles(maxCycles);
            pausableStochasticAI.setCyclesPerIteration(cyclesPerIteration);

            static auto start = std::chrono::steady_clock::now();
            static auto end = std::chrono::steady_clock::now();

            if (ImGui::Button("Stochastic AI move")) {
                start = std::chrono::steady_clock::now();

                pausableStochasticAI.setGameAndResetState(game, currentPlayer);
                isCalculating = true;
            }

            if (isCalculating) {
                pausableStochasticAI.think(random);
                
                if (pausableStochasticAI.isFinished()) {
                    gameMove = pausableStochasticAI.getBestMove();
                    
                    if (gameMove.moveType == GameMoveType::MOVE) {
                        game.applyMove(currentPlayer, gameMove.move, random);
                    } else {
                        std::cout << "Move was: " << (gameMove.moveType == GameMoveType::SKIP ? "SKIP" : "EXCHANGE") << "\n";
                    }
                    
                    currentPlayer = (currentPlayer + 1) % game.getNumPlayers();

                    isCalculating = false;
                }
                
                end = std::chrono::steady_clock::now();
            }

            ImGui::Text("Duration: %ld", 
                    std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                    );

            {
                std::string res;
                if (gameMove.moveType == GameMoveType::SKIP) {
                    res = "skip";
                } else if (gameMove.moveType == GameMoveType::EXCHANGE) {
                    res = "exchange";
                } else if (gameMove.moveType == GameMoveType::MOVE) {
                    res = "move";
                }
                ImGui::Text("Was: %s", res.c_str());
            }


            ImVec2 windowSize = ImGui::GetWindowSize();
            auto winCount = normalize(pausableStochasticAI.getWinCount());

            ImGui::PlotHistogram("Test histogram", winCount.data.data(), winCount.getSize(), 0, NULL, 0, FLT_MAX, ImVec2(windowSize.x, 200));
            ImGui::ProgressBar(pausableStochasticAI.getProgress());

            ImGui::End();
        }

        gameStateWindow.updateState(game);
        gameStateWindow.render();

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
