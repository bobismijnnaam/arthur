#include <vector>
#include <SDL.h>

#include "Spectrangle.h"
#include "TriangleRenderer.h"
#include "TileTexture.h"
#include "SpectrangleTexture.h"

class SpectrangleGameStateWindow {
public:
    SpectrangleGameStateWindow(char const * windowTitle, SDL_Window* window, SDL_GLContext scratchContext);

    void updateState(Spectrangle const & game);
    void render();
    
private:
    char const * windowTitle;
    SDL_GLContext scratchContext;

    bool hadFirstRender;

    TriangleRenderer triangleRenderer;
    SpectrangleTexture spectrangleTexture;
    std::array<std::vector<TileTexture>, MAX_NUM_PLAYERS> tileTextures;

    Vec2i inputPos;
     
    int numPlayers;
} ;

