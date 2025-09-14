#include "Arvore.h"
#include "CircleFilled.h"
#include <cmath>

Arvore::Arvore(double _x,double _y,double h, Color t, Color f)
    : x(_x), y(_y), altura(h), corTronco(t), corFolha(f) {}

void Arvore::draw(SDL_Renderer* renderer, Transform& T) {
    SDL_Point base = T.toSRD(x,y);

    // tronco
    double trunkWm = altura * 0.15;
    double trunkHm = altura * 0.35;
    int trunkW = std::max(1, (int)round(trunkWm * T.sx));
    int trunkH = std::max(1, (int)round(trunkHm * T.sy));

    // calcula top do tronco; se for negativo, faz clamp para 0 (assim fica visível)
    int trunkTopY = base.y - trunkH;
    if (trunkTopY < 0) trunkTopY = 0;

    SDL_Rect tronco = { base.x - trunkW/2, trunkTopY, trunkW, trunkH };
    SDL_SetRenderDrawColor(renderer, corTronco.r, corTronco.g, corTronco.b, corTronco.a);
    SDL_RenderFillRect(renderer, &tronco);

    // copa (3 círculos) — posiciona relativamente ao topo do tronco
    int radius = std::max(1, (int)round(altura * T.sx * 0.25));
    int copaCenterY = trunkTopY - radius/2;
    // se copa acima do topo, coloca a copa logo abaixo do topo para garantir visibilidade
    if (copaCenterY < 0) copaCenterY = trunkTopY + radius/2;

    SDL_SetRenderDrawColor(renderer, corFolha.r, corFolha.g, corFolha.b, corFolha.a);
    fillCircle(renderer, base.x, copaCenterY, radius);
    fillCircle(renderer, base.x - radius/2, copaCenterY + radius/4, radius);
    fillCircle(renderer, base.x + radius/2, copaCenterY + radius/4, radius);

    // marcador visual discreto para debug (sem imprimir no console)
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect marker = { base.x - 2, base.y - 2, 4, 4 };
    SDL_RenderFillRect(renderer, &marker);
}
