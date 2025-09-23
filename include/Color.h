#ifndef COLOR_H
#define COLOR_H

#include <SDL2/SDL.h>
#include <string>
#include <map>

// Mapeamento de nomes de cores para valores RGB
struct Color { Uint8 r,g,b,a; };

inline std::map<std::string, Color> colorTable = {
    {"Preto",{0,0,0,255}}, {"Branco",{255,255,255,255}}, {"Vermelho",{255,0,0,255}},
    {"Verde",{0,128,0,255}}, {"Azul",{0,0,255,255}}, {"Amarela",{255,255,0,255}},
    {"Marrom",{150,75,0,255}}, {"Cinza",{128,128,128,255}}, {"Roxo",{128,0,128,255}},
    {"Laranja",{255,165,0,255}}, {"Rosa",{255,192,203,255}}, {"Ciano",{0,255,255,255}},
    {"Magenta",{255,0,255,255}}, {"Oliva",{128,128,0,255}}, {"Lima", {0,255,0,255}},  {"Navy",{0,0,128,255}},
    {"Marfim",{255,255,240,255}}
};

inline Color parseColor(const std::string& name){
    auto it = colorTable.find(name);
    if(it!=colorTable.end()) return it->second;
    return {255,255,255,255}; // fallback branco
}

#endif
