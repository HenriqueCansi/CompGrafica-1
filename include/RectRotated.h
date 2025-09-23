#ifndef RECTROTATED_H
#define RECTROTATED_H

#include "Shape.h"
#include "Color.h"
#include <SDL2/SDL.h>

class RectRotated : public Shape {
    SDL_Point origin;     // ponto base de rotação
    int width, height;    // dimensões em pixels
    double angle;         // ângulo em graus
    Color color;
public:
    RectRotated(SDL_Point base, int w, int h, double ang, Color c);
    void draw(SDL_Renderer* renderer, Transform& T) override;
};

#endif
