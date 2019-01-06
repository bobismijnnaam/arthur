#include <iostream>

#include "SpectrangleGameStateWindow.h"
#include "imgui.h"

SpectrangleGameStateWindow::SpectrangleGameStateWindow(char const * windowTitleArg, SDL_Window* window, SDL_GLContext contextArg) : 
        windowTitle{windowTitleArg},
        scratchContext{contextArg},
        triangleRenderer(window, contextArg),
        spectrangleTexture(500, 500, 0.33, window, scratchContext, triangleRenderer) {
    
    for (int y = 0; y < MAX_NUM_PLAYERS; y++) {
        std::vector<TileTexture>& playerTiles = tileTextures[y];
        for (int x = 0; x < MAX_TILES_PER_PLAYER; x++) {
            playerTiles.push_back(TileTexture(100, 100, window, scratchContext, triangleRenderer));
        }
    }

    hadFirstRender = false;
}

void SpectrangleGameStateWindow::updateState(TileBoard const & board, PlayersState const & playerBags) {
    hadFirstRender = true;

    spectrangleTexture.updateState(board);

    for (int player = 0; player < MAX_NUM_PLAYERS; player++) {
        PlayerBag const & bag = playerBags[player];
        for (int tileIndex = 0; tileIndex < bag.getSize(); tileIndex++) {
            tileTextures[player][tileIndex].updateState(bag[tileIndex]);
        }
    }
}

void SpectrangleGameStateWindow::render() {
    ImGui::Begin(windowTitle);                          

    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 framePadding(10, 10);

    if (hadFirstRender) {
        ImGui::Image(spectrangleTexture.getTexture(), ImVec2(windowSize.x - framePadding.x * 2, windowSize.x - framePadding.x * 2), ImVec2(0,1), ImVec2(1,0), ImColor(255,255,255,255), ImColor(255,255,255,128));

        for (int player = 0; player < MAX_NUM_PLAYERS; player++) {
            ImGui::Text("Player %d:", player);
            for (int tileIndex = 0; tileIndex < MAX_TILES_PER_PLAYER; tileIndex++) {
                ImGui::SameLine();

                TileTexture& tile = tileTextures[player][tileIndex];

                ImGui::ImageButton(tile.getTexture(), ImVec2(30,30), ImVec2(0,1), ImVec2(1, 0), 1, ImColor(0,0,0,255));
            }
        }
    }

    ImGui::SliderInt("X pos", &inputPos.x, 0, TileBoard::rowLength(inputPos.y) - 1);
    ImGui::SliderInt("Y pos", &inputPos.y, 0, SPECTRANGLE_BOARD_SIDE - 1);

    if (inputPos.y < 0) inputPos.y = 0;
    if (inputPos.x < 0) inputPos.x = 0;

    if (inputPos.y >= SPECTRANGLE_BOARD_SIDE) inputPos.y = SPECTRANGLE_BOARD_SIDE - 1;
    if (inputPos.x >= TileBoard::rowLength(inputPos.y)) inputPos.x = TileBoard::rowLength(inputPos.y) - 1;

    ImGui::End();
}

