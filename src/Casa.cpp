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

    // --- PAREDE ---
    SDL_SetRenderDrawColor(renderer, corParede.r, corParede.g, corParede.b, corParede.a);
    SDL_Rect parede = {px, py, w, h};
    SDL_RenderFillRect(renderer, &parede);

    // --- TELHADO (triângulo preenchido) ---
    // pontos do triângulo em SRD:
    int x0 = px;
    int y0 = py;
    int x1 = px + w;
    int y1 = py;
    int xm = px + w/2;
    int ym = py - (h / 2); // altura do telhado = metade da altura da casa (ajusta se quiseres)

    // bounding box vertical
    int ymin = std::min({y0,y1,ym});
    int ymax = std::max({y0,y1,ym});

    SDL_SetRenderDrawColor(renderer, corTelhado.r, corTelhado.g, corTelhado.b, corTelhado.a);
    // scanline fill: para cada linha Y encontra interseções com arestas do triângulo
    auto interp = [](int ay,int ax,int by,int bx,int y)->int{
        if(by==ay) return ax;
        double t = double(y - ay) / double(by - ay);
        return (int)round(ax + t*(bx-ax));
    };

    for(int yy = ymin; yy <= ymax; ++yy){
        std::vector<int> xs;
        xs.push_back(interp(y0,x0,ym,xm,yy));
        xs.push_back(interp(y1,x1,ym,xm,yy));
        xs.push_back(interp(y0,x0,y1,x1,yy));
        int xmin = *std::min_element(xs.begin(), xs.end());
        int xmax = *std::max_element(xs.begin(), xs.end());
        // desenha linha horizontal preenchendo a faixa
        SDL_RenderDrawLine(renderer, xmin, yy, xmax, yy);
    }

    // desenhar contorno do telhado (opcional)
    SDL_SetRenderDrawColor(renderer, 0,0,0,255); // contorno a preto
    SDL_RenderDrawLine(renderer, x0, y0, xm, ym);
    SDL_RenderDrawLine(renderer, x1, y1, xm, ym);
    SDL_RenderDrawLine(renderer, x0, y0, x1, y1);

    // --- PORTA ---
    int portaW = (int)round(w * 0.25);
    int portaH = (int)round(h * 0.4);
    int portaX = px + (w - portaW)/2;
    int portaY = py + h - portaH;
    SDL_SetRenderDrawColor(renderer, corPorta.r, corPorta.g, corPorta.b, corPorta.a);
    SDL_Rect porta = {portaX, portaY, portaW, portaH};
    SDL_RenderFillRect(renderer, &porta);

    // opcional: contorno da parede
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderDrawRect(renderer, &parede);
}
