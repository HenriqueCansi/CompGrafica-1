#include "RectRotated.h"
#include "TransformUtils.h"
#include <cmath>
#include <vector>
#include <algorithm>

// --------------- Função auxiliar - Preencher quadrilátero ------------------
static void fillQuad(SDL_Renderer* renderer, SDL_Point p1, SDL_Point p2, SDL_Point p3, SDL_Point p4) {
    SDL_Point pts[4] = {p1, p2, p3, p4};
    int minY = std::min({p1.y, p2.y, p3.y, p4.y});
    int maxY = std::max({p1.y, p2.y, p3.y, p4.y});

    for (int y = minY; y <= maxY; y++) {
        std::vector<int> intersec;
        for (int i = 0; i < 4; i++) {
            SDL_Point a = pts[i];
            SDL_Point b = pts[(i+1)%4];
            if ((a.y <= y && b.y > y) || (b.y <= y && a.y > y)) {
                int x = a.x + (y - a.y) * (b.x - a.x) / (b.y - a.y);
                intersec.push_back(x);
            }
        }
        std::sort(intersec.begin(), intersec.end());
        for (size_t k = 0; k+1 < intersec.size(); k+=2) {
            SDL_RenderDrawLine(renderer, intersec[k], y, intersec[k+1], y);
        }
    }
}

RectRotated::RectRotated(SDL_Point base, int w, int h, double ang, Color c)
    : origin(base), width(w), height(h), angle(ang), color(c) {}

void RectRotated::draw(SDL_Renderer* renderer, Transform& T) {
    // Cantos do retângulo antes da rotação
    SDL_Point p1 = { origin.x - width/2, origin.y };
    SDL_Point p2 = { origin.x + width/2, origin.y };
    SDL_Point p3 = { origin.x + width/2, origin.y - height };
    SDL_Point p4 = { origin.x - width/2, origin.y - height };

    // Aplica rotação
    p1 = rotatePoint(p1, origin, angle);
    p2 = rotatePoint(p2, origin, angle);
    p3 = rotatePoint(p3, origin, angle);
    p4 = rotatePoint(p4, origin, angle);

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    fillQuad(renderer, p1, p2, p3, p4);
}
