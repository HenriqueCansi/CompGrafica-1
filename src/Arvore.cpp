#include "Arvore.h"
#include "CircleFilled.h"
#include "RectRotated.h"
#include <cmath>
#include <algorithm>

Arvore::Arvore(double _x,double _y,double h, Color t, Color f, double ang)
    : x(_x), y(_y), altura(h), inclinacao(ang), corTronco(t), corFolha(f) {}

void Arvore::draw(SDL_Renderer* renderer, Transform& T) {
    SDL_Point base = T.toSRD(x,y);

    // dimensões do tronco em pixels
    int trunkW = std::max(1, (int)round(altura * 0.15 * T.sx));
    int trunkH = std::max(1, (int)round(altura * 0.35 * T.sy));

    // usa RectRotated para o tronco
    RectRotated tronco(base, trunkW, trunkH, inclinacao, corTronco);
    tronco.draw(renderer, T);

    // copa (3 círculos, rotacionados em torno da base)
    int radius = std::max(1, (int)round(altura * T.sx * 0.25));
    SDL_Point center = { base.x, base.y - trunkH - radius/2 };

    // aplica rotação do centro da copa
    extern SDL_Point rotatePoint(SDL_Point p, SDL_Point origin, double angGraus);
    SDL_Point c1 = rotatePoint(center, base, inclinacao);
    SDL_Point c2 = rotatePoint({center.x - radius/2, center.y + radius/4}, base, inclinacao);
    SDL_Point c3 = rotatePoint({center.x + radius/2, center.y + radius/4}, base, inclinacao);

    SDL_SetRenderDrawColor(renderer, corFolha.r, corFolha.g, corFolha.b, corFolha.a);
    fillCircle(renderer, c1.x, c1.y, radius);
    fillCircle(renderer, c2.x, c2.y, radius);
    fillCircle(renderer, c3.x, c3.y, radius);
}
