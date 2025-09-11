#ifndef SOL_H
#define SOL_H

#include "Shape.h"
#include "Color.h"

class Sol : public Shape {
    double x,y, raio;
    Color cor;
public:
    Sol(double _x,double _y,double r, Color c);
    void draw(SDL_Renderer* renderer, Transform& T) override;
};

#endif
