#ifndef SHAPE_H
#define SHAPE_H

#include <SDL2/SDL.h>
#include "Transformations.h"

class Shape {
public:
    virtual ~Shape() {}
    virtual void draw(SDL_Renderer* renderer, Transform& T) = 0;
};

#endif
