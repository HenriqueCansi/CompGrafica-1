#include "Casa.h"
#include "TransformUtils.h"
#include <algorithm>
#include <vector>
#include <cmath>

static void fillPolygon(SDL_Renderer* renderer, const std::vector<SDL_Point>& verts) {
    if (verts.size() < 3) return;

    int minY = verts[0].y, maxY = verts[0].y;
    for (auto& v : verts) {
        if (v.y < minY) minY = v.y;
        if (v.y > maxY) maxY = v.y;
    }

    for (int y = minY; y <= maxY; y++) {
        std::vector<int> nodes;
        for (size_t i = 0, j = verts.size() - 1; i < verts.size(); j = i++) {
            int yi = verts[i].y, yj = verts[j].y;
            int xi = verts[i].x, xj = verts[j].x;

            if ((yi <= y && yj > y) || (yj <= y && yi > y)) {
                double x = xi + (y - yi) * (double)(xj - xi) / (double)(yj - yi);
                nodes.push_back((int)std::round(x));
            }
        }

        if (nodes.empty()) continue;
        std::sort(nodes.begin(), nodes.end());
        for (size_t k = 0; k + 1 < nodes.size(); k += 2) {
            int xA = nodes[k];
            int xB = nodes[k + 1];
            if (xA <= xB) SDL_RenderDrawLine(renderer, xA, y, xB, y);
        }
    }
}

Casa::Casa(double _x,double _y,double h,double w, Color p, Color t, Color porta, double ang)
    : x(_x), y(_y), altura(h), largura(w),
      corParede(p), corTelhado(t), corPorta(porta),
      inclinacao(ang) {}

void Casa::draw(SDL_Renderer* renderer, Transform& T) {
    SDL_Point bl = T.toSRD(x, y);
    int W = (int)round(largura * T.sx);
    int H = (int)round(altura * T.sy);
    int px = bl.x;
    int py = bl.y - H;

    // piv� no canto inferior esquerdo
    SDL_Point pivot = { px, py + H };

    // propor��es da parede e telhado
    int paredeH = (int)(H * 0.7);
    int telhadoH = H - paredeH;

    // ------------------ PAREDE ------------------
    std::vector<SDL_Point> parede = {
        { px,       py + telhadoH },            // topo-esq (com base no in�cio do telhado)
        { px+W,     py + telhadoH },            // topo-dir
        { px+W,     py + telhadoH + paredeH },  // base-dir
        { px,       py + telhadoH + paredeH }   // base-esq
    };
    for (auto &p : parede) p = rotatePoint(p, pivot, inclinacao);

    SDL_SetRenderDrawColor(renderer, corParede.r, corParede.g, corParede.b, corParede.a);
    fillPolygon(renderer, parede);

    // ------------------ TELHADO ------------------
    std::vector<SDL_Point> telhado = {
        { px,       py + telhadoH },        // topo-esq da parede
        { px+W,     py + telhadoH },        // topo-dir da parede
        { px+W/2,   py }                    // pico do telhado (em cima)
    };
    for (auto &p : telhado) p = rotatePoint(p, pivot, inclinacao);

    SDL_SetRenderDrawColor(renderer, corTelhado.r, corTelhado.g, corTelhado.b, corTelhado.a);
    fillPolygon(renderer, telhado);

    // ------------------ PORTA ------------------
    int portaW = W / 4;
    int portaH = paredeH / 3;
    std::vector<SDL_Point> porta = {
        { px + W/2 - portaW/2, py + telhadoH + paredeH - portaH },
        { px + W/2 + portaW/2, py + telhadoH + paredeH - portaH },
        { px + W/2 + portaW/2, py + telhadoH + paredeH },
        { px + W/2 - portaW/2, py + telhadoH + paredeH }
    };
    for (auto &p : porta) p = rotatePoint(p, pivot, inclinacao);

    SDL_SetRenderDrawColor(renderer, corPorta.r, corPorta.g, corPorta.b, corPorta.a);
    fillPolygon(renderer, porta);

    // ------------------ JANELAS ------------------
    int janelaW = W / 5;
    int janelaH = paredeH / 4;
    int janelaY = py + telhadoH + paredeH/3;

    SDL_Color corJanela = {135, 206, 235, 255};

    // esquerda
    std::vector<SDL_Point> janelaEsq = {
        { px + W/4 - janelaW/2, janelaY },
        { px + W/4 + janelaW/2, janelaY },
        { px + W/4 + janelaW/2, janelaY + janelaH },
        { px + W/4 - janelaW/2, janelaY + janelaH }
    };
    for (auto &p : janelaEsq) p = rotatePoint(p, pivot, inclinacao);

    SDL_SetRenderDrawColor(renderer, corJanela.r, corJanela.g, corJanela.b, corJanela.a);
    fillPolygon(renderer, janelaEsq);

    // cruz esquerda
    SDL_Point jEsqTop = rotatePoint({px + W/4, janelaY}, pivot, inclinacao);
    SDL_Point jEsqBot = rotatePoint({px + W/4, janelaY + janelaH}, pivot, inclinacao);
    SDL_Point jEsqEsq = rotatePoint({px + W/4 - janelaW/2, janelaY + janelaH/2}, pivot, inclinacao);
    SDL_Point jEsqDir = rotatePoint({px + W/4 + janelaW/2, janelaY + janelaH/2}, pivot, inclinacao);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawLine(renderer, jEsqEsq.x, jEsqEsq.y, jEsqDir.x, jEsqDir.y);
    SDL_RenderDrawLine(renderer, jEsqTop.x, jEsqTop.y, jEsqBot.x, jEsqBot.y);

    // direita
    std::vector<SDL_Point> janelaDir = {
        { px + 3*W/4 - janelaW/2, janelaY },
        { px + 3*W/4 + janelaW/2, janelaY },
        { px + 3*W/4 + janelaW/2, janelaY + janelaH },
        { px + 3*W/4 - janelaW/2, janelaY + janelaH }
    };
    for (auto &p : janelaDir) p = rotatePoint(p, pivot, inclinacao);

    SDL_SetRenderDrawColor(renderer, corJanela.r, corJanela.g, corJanela.b, corJanela.a);
    fillPolygon(renderer, janelaDir);

    // cruz direita
    SDL_Point jDirTop = rotatePoint({px + 3*W/4, janelaY}, pivot, inclinacao);
    SDL_Point jDirBot = rotatePoint({px + 3*W/4, janelaY + janelaH}, pivot, inclinacao);
    SDL_Point jDirEsq = rotatePoint({px + 3*W/4 - janelaW/2, janelaY + janelaH/2}, pivot, inclinacao);
    SDL_Point jDirDir = rotatePoint({px + 3*W/4 + janelaW/2, janelaY + janelaH/2}, pivot, inclinacao);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawLine(renderer, jDirEsq.x, jDirEsq.y, jDirDir.x, jDirDir.y);
    SDL_RenderDrawLine(renderer, jDirTop.x, jDirTop.y, jDirBot.x, jDirBot.y);
}
