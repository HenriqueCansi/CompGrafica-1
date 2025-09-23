#include "Sol.h"
#include "CircleFilled.h"
#include <cmath>

Sol::Sol(double _x,double _y,double r, Color c, double ang)
    : x(_x), y(_y), raio(r), inclinacao(ang), cor(c) {}

void Sol::draw(SDL_Renderer* renderer, Transform& T) {
    SDL_Point c = T.toSRD(x,y);
    int rad = (int)(raio * T.sx);

    // corpo do sol
    SDL_SetRenderDrawColor(renderer, cor.r, cor.g, cor.b, cor.a);
    fillCircle(renderer, c.x, c.y, rad);

    // raios solares rotacionados por "inclinacao"
    for(int i=0; i<12; i++) {
        double ang = i * (2*M_PI/12) + (inclinacao * M_PI / 180.0); // deslocamento
        int x2 = c.x + (int)(rad*1.5 * cos(ang));
        int y2 = c.y + (int)(rad*1.5 * sin(ang));
        SDL_RenderDrawLine(renderer, c.x, c.y, x2, y2);
    }
}
