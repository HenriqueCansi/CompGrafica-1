#include "Arvore.h"
#include "CircleFilled.h"

Arvore::Arvore(double _x,double _y,double h, Color t, Color f)
    : x(_x), y(_y), altura(h), corTronco(t), corFolha(f) {}

void Arvore::draw(SDL_Renderer* renderer, Transform& T) {
    SDL_Point base = T.toSRD(x,y);

    // tronco
    double trunkWm = altura * 0.15;
    double trunkHm = altura * 0.35;
    int trunkW = (int)(trunkWm * T.sx);
    int trunkH = (int)(trunkHm * T.sy);
    SDL_Rect tronco = { base.x - trunkW/2, base.y - trunkH, trunkW, trunkH };
    SDL_SetRenderDrawColor(renderer, corTronco.r, corTronco.g, corTronco.b, corTronco.a);
    SDL_RenderFillRect(renderer, &tronco);

    // copa (3 círculos sobrepostos)
    int radius = (int)(altura * T.sx * 0.25);
    SDL_SetRenderDrawColor(renderer, corFolha.r, corFolha.g, corFolha.b, corFolha.a);
    fillCircle(renderer, base.x, base.y - trunkH, radius);
    fillCircle(renderer, base.x - radius/2, base.y - trunkH - radius/2, radius);
    fillCircle(renderer, base.x + radius/2, base.y - trunkH - radius/2, radius);

}
