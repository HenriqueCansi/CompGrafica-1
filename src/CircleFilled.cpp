#include "CircleFilled.h"
#include <cmath>

void fillCircle(SDL_Renderer* renderer, int cx, int cy, int radius) {
    for(int dy=-radius; dy<=radius; dy++){
        int dx = (int)std::sqrt(radius*radius - dy*dy);
        SDL_RenderDrawLine(renderer, cx-dx, cy+dy, cx+dx, cy+dy);
    }
}
