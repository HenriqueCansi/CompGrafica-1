#ifndef TRANSFORMUTILS_H
#define TRANSFORMUTILS_H

#include <SDL2/SDL.h>

// Rotaciona ponto em torno de uma origem
SDL_Point rotatePoint(SDL_Point p, SDL_Point origin, double angGraus);

#endif
