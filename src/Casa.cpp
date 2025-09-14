#include "Casa.h"
#include <algorithm>
#include <vector>
#include <cmath>

Casa::Casa(double _x,double _y,double h,double w, Color p, Color t, Color porta)
    : x(_x), y(_y), altura(h), largura(w), corParede(p), corTelhado(t), corPorta(porta) {}

void Casa::draw(SDL_Renderer* renderer, Transform& T) {
    SDL_Point bl = T.toSRD(x,y);
    int w = (int)round(largura * T.sx);
    int h = (int)round(altura * T.sy);
    int px = bl.x;
    int py = bl.y - h;

    // parede
    SDL_Rect parede = { px, py, w, h };
    SDL_SetRenderDrawColor(renderer, corParede.r, corParede.g, corParede.b, corParede.a);
    SDL_RenderFillRect(renderer, &parede);

    // telhado como triângulo preenchido (scanline fill)
    int x1 = px,     y1 = py;
    int x2 = px+w,   y2 = py;
    int x3 = px+w/2, y3 = py - h/2;

    SDL_SetRenderDrawColor(renderer, corTelhado.r, corTelhado.g, corTelhado.b, corTelhado.a);

    // ordena os vértices por Y (y1 <= y2 <= y3)
    if (y2 < y1) { std::swap(y1,y2); std::swap(x1,x2); }
    if (y3 < y1) { std::swap(y1,y3); std::swap(x1,x3); }
    if (y3 < y2) { std::swap(y2,y3); std::swap(x2,x3); }

    auto edgeInterp = [](int x0,int y0,int x1,int y1,int y){
        if(y1==y0) return x0;
        return x0 + (x1-x0)*(y-y0)/(y1-y0);
    };

    for(int y=y1; y<=y3; y++){
        int xa, xb;
        if(y<y2){
            xa = edgeInterp(x1,y1,x3,y3,y);
            xb = edgeInterp(x1,y1,x2,y2,y);
        } else {
            xa = edgeInterp(x1,y1,x3,y3,y);
            xb = edgeInterp(x2,y2,x3,y3,y);
        }
        if(xa>xb) std::swap(xa,xb);
        SDL_RenderDrawLine(renderer, xa,y, xb,y);
    }

    // porta
    int portaW = w/4;
    int portaH = h/3;
    SDL_Rect porta = { px + w/2 - portaW/2, py + h - portaH, portaW, portaH };
    SDL_SetRenderDrawColor(renderer, corPorta.r, corPorta.g, corPorta.b, corPorta.a);
    SDL_RenderFillRect(renderer, &porta);
}
