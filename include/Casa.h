#ifndef CASA_H
#define CASA_H

#include "Shape.h"
#include "Rectangle.h"
#include "Color.h"
#include <vector>
#include <memory>

class Casa : public Shape {
    double x,y, altura, largura;
    Color corParede, corTelhado, corPorta;
public:
    Casa(double _x,double _y,double h,double w, Color p, Color t, Color porta);
    void draw(SDL_Renderer* renderer, Transform& T) override;
};

#endif

