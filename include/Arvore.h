#ifndef ARVORE_H
#define ARVORE_H

#include "Shape.h"
#include "Color.h"

class Arvore : public Shape {
    double x,y, altura;
    Color corTronco, corFolha;
public:
    Arvore(double _x,double _y,double h, Color t, Color f);
    void draw(SDL_Renderer* renderer, Transform& T) override;
};

#endif
