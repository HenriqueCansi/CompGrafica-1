#ifndef CERCA_H
#define CERCA_H

#include "Shape.h"
#include "Color.h"

class Cerca : public Shape {
    double x,y, largura, altura;
    Color cor;
public:
    Cerca(double _x,double _y,double w,double h, Color c);
    void draw(SDL_Renderer* renderer, Transform& T) override;
};

#endif
