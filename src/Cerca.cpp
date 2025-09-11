#include "Cerca.h"

Cerca::Cerca(double _x,double _y,double w,double h, Color c)
    : x(_x), y(_y), largura(w), altura(h), cor(c) {}

void Cerca::draw(SDL_Renderer* renderer, Transform& T) {
    SDL_Point base = T.toSRD(x,y);
    int w = (int)(largura * T.sx);
    int h = (int)(altura * T.sy);

    SDL_SetRenderDrawColor(renderer, cor.r, cor.g, cor.b, cor.a);

    // postes
    int posts = (int)(largura) + 1;
    for(int i=0;i<posts;i++){
        double xm = x + (largura/(posts-1))*i;
        SDL_Point p = T.toSRD(xm,y);
        SDL_Rect post = { p.x-2, p.y-h, 4, h };
        SDL_RenderFillRect(renderer,&post);
    }

    // travessas horizontais
    int railY1 = base.y - h/3;
    int railY2 = base.y - 2*h/3;
    SDL_RenderDrawLine(renderer, base.x, railY1, base.x+w, railY1);
    SDL_RenderDrawLine(renderer, base.x, railY2, base.x+w, railY2);
}
