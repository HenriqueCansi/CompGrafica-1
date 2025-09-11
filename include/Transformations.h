#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H


#include <SDL2/SDL.h>
#include <cmath>
// Função para converter coordenadas do universo (metros) para coordenadas de tela (pixels)
// SRU (Screen Real-world Units) para SRD (Screen Resolution Dimensions)
// Assumindo que (0,0) da tela é superior esquerdo e (0,0) do universo é inferior esquerdo
// e que a origem do universo (0,0) está no canto inferior esquerdo da tela.
// A resolução da tela e as dimensões do universo em metros são passadas como parâmetros.

// x_sru, y_sru: Coordenadas do objeto no universo (metros)
// screenWidth, screenHeight: Resolução da tela em pixels
// universeWidth, universeHeight: Dimensões do universo em metros


struct Transform {
    double sruWidthMeters, sruHeightMeters;
    int srdWidthPx, srdHeightPx;
    double sx, sy;

    void init(double sruW, double sruH, int srdW, int srdH) {
        sruWidthMeters = sruW;
        sruHeightMeters = sruH;
        srdWidthPx = srdW;
        srdHeightPx = srdH;
        sx = (double)srdW / sruW;
        sy = (double)srdH / sruH;
    }

    SDL_Point toSRD(double x_m, double y_m) {
        int x = (int)round(x_m * sx);
        int y = (int)round(srdHeightPx - (y_m * sy));
        return {x,y};
    }
};

#endif


