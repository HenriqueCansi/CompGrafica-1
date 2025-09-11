#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <SDL2/SDL.h>

inline void fillRect(SDL_Renderer* r,int x,int y,int w,int h){
    SDL_Rect rect = {x,y,w,h};
    SDL_RenderFillRect(r,&rect);
}

#endif
