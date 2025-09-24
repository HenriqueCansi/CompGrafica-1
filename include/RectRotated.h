#ifndef RECTROTATED_H
#define RECTROTATED_H

#include "Shape.h"
#include "Color.h"
#include <SDL2/SDL.h>

class RectRotated : public Shape {
    SDL_Point origin;     // Ponto base de rotação
    int width, height;    // Dimensões em pixels
    double angle;         // Ângulo em graus
    Color color;
public:
    RectRotated(SDL_Point base, int w, int h, double ang, Color c);
    void draw(SDL_Renderer* renderer, Transform& T) override;
};

#endif
