#include "Cerca.h"
#include "RectRotated.h"
#include "TransformUtils.h"
#include <cmath>
#include <algorithm>

Cerca::Cerca(double _x,double _y,double w,double h, Color c, double ang)
    : x(_x), y(_y), largura(w), altura(h), inclinacao(ang), cor(c) {}

void Cerca::draw(SDL_Renderer* renderer, Transform& T) {
    SDL_Point base = T.toSRD(x,y);
    int w = (int)(largura * T.sx);
    int h = (int)(altura * T.sy);

    SDL_SetRenderDrawColor(renderer, cor.r, cor.g, cor.b, cor.a);


    // ------------------ POSTES ------------------
    int posts = (int)(largura) + 1;
    for(int i=0; i<posts; i++) {
        double xm = x + (largura/(posts-1)) * i;
        // Centro do poste em coordenadas do mundo
        SDL_Point posteBase = T.toSRD(xm, y);

        // Aplica a rotação em torno do ponto base (origem da cerca)
        posteBase = rotatePoint(posteBase, base, inclinacao);

        // Desenha poste inclinado E rotacionado na posição correta
        RectRotated post(posteBase, 4, h, inclinacao, cor);
        post.draw(renderer, T);

    }


    // -------------- TRAVESSAS HORIZONTAIS ---------------
    //             (rotacionadas manualmente)
    SDL_Point r1a = { base.x, base.y - h/3 };
    SDL_Point r1b = { base.x + w, base.y - h/3 };
    SDL_Point r2a = { base.x, base.y - 2*h/3 };
    SDL_Point r2b = { base.x + w, base.y - 2*h/3 };

    r1a = rotatePoint(r1a, base, inclinacao);
    r1b = rotatePoint(r1b, base, inclinacao);
    r2a = rotatePoint(r2a, base, inclinacao);
    r2b = rotatePoint(r2b, base, inclinacao);

    SDL_RenderDrawLine(renderer, r1a.x, r1a.y, r1b.x, r1b.y);
    SDL_RenderDrawLine(renderer, r2a.x, r2a.y, r2b.x, r2b.y);
}
