#include "App.h"
#include "Casa.h"
#include "Arvore.h"
#include "Cerca.h"
#include "Sol.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

App::App() : window(nullptr), renderer(nullptr), running(true),
             width(800), height(600), metersW(40), metersH(30), bg({0,0,255,255}) {}

App::~App(){
    if(renderer) SDL_DestroyRenderer(renderer);
    if(window) SDL_DestroyWindow(window);
    SDL_Quit();
}

std::vector<std::string> App::tokenizeCSV(const std::string& filename){
    std::ifstream in(filename);
    std::vector<std::string> tokens;
    if(!in){ std::cerr<<"Erro ao abrir "<<filename<<"\n"; return tokens; }
    std::string line;
    while(std::getline(in,line)){
        std::stringstream ss(line);
        std::string tok;
        while(std::getline(ss,tok,';')){
            if(!tok.empty()) tokens.push_back(tok);
        }
    }
    return tokens;
}

void App::parseCSV(const std::string& filename){
    auto tokens = tokenizeCSV(filename);
    size_t i=0;
    while(i < tokens.size()){
        std::string tk = tokens[i++];
        if(tk == "Tela"){
            width = std::stoi(tokens[i+1]);
            height = std::stoi(tokens[i+2]);
            i+=3;
         } else if(tk == "Casa") {
                double x=0,y=0,alt=3,larg=3;
                std::string corParede="Branco", corTelhado="Vermelho", corPorta="Marrom";

                while (i < tokens.size()) {
                    if(tokens[i] == "Casa" || tokens[i] == "Arvore" || tokens[i] == "Cerca" || tokens[i] == "Sol") break;
                    std::string p = tokens[i++];
                    if(p=="Localização" || p=="Localizacao") {
                        x = std::stod(tokens[i++]);
                        y = std::stod(tokens[i++]);
                    } else if(p=="Altura") {
                        alt = std::stod(tokens[i++]);
                    } else if(p=="Largura") {
                        larg = std::stod(tokens[i++]);
                    } else if(p=="CorParede") {
                        corParede = tokens[i++];
                    } else if(p=="CorTelhado") {
                        corTelhado = tokens[i++];
                    } else if(p=="CorPorta") {
                        corPorta = tokens[i++];
                    }
        }

        scene.push_back(std::make_unique<Casa>(
            x,y,alt,larg,
            parseColor(corParede),
            parseColor(corTelhado),
            parseColor(corPorta)
        ));
    } else if(tk == "Arvore") {
            double x=0,y=0,alt=3;
            std::string corTronco="Marrom", corFolha="Verde";

            while (i < tokens.size()) {
                if(tokens[i]=="Casa"||tokens[i]=="Arvore"||tokens[i]=="Cerca"||tokens[i]=="Sol") break;
                std::string p = tokens[i++];
                if(p=="Localização"||p=="Localizacao"){ x=std::stod(tokens[i++]); y=std::stod(tokens[i++]); }
                else if(p=="Altura"){ alt=std::stod(tokens[i++]); }
                else if(p=="CorTronco"){ corTronco=tokens[i++]; }
                else if(p=="CorFolha"){ corFolha=tokens[i++]; }
            }

            scene.push_back(std::make_unique<Arvore>(x,y,alt,
                parseColor(corTronco), parseColor(corFolha)));
        } else if(tk == "Cerca") {
            double x=0,y=0,larg=5,alt=1;
            std::string cor="Marrom";

            while (i < tokens.size()) {
                if(tokens[i]=="Casa"||tokens[i]=="Arvore"||tokens[i]=="Cerca"||tokens[i]=="Sol") break;
                std::string p = tokens[i++];
                if(p=="Localização"||p=="Localizacao"){ x=std::stod(tokens[i++]); y=std::stod(tokens[i++]); }
                else if(p=="Largura"){ larg=std::stod(tokens[i++]); }
                else if(p=="Altura"){ alt=std::stod(tokens[i++]); }
                else if(p=="Cor"){ cor=tokens[i++]; }
            }

    scene.push_back(std::make_unique<Cerca>(x,y,larg,alt,parseColor(cor)));
} else if(tk == "Sol") {
    double x=0,y=0,r=2;
    std::string cor="Amarelo";

    while (i < tokens.size()) {
        if(tokens[i]=="Casa"||tokens[i]=="Arvore"||tokens[i]=="Cerca"||tokens[i]=="Sol") break;
        std::string p = tokens[i++];
        if(p=="Localização"||p=="Localizacao"){ x=std::stod(tokens[i++]); y=std::stod(tokens[i++]); }
        else if(p=="Raio"){ r=std::stod(tokens[i++]); }
        else if(p=="Cor"){ cor=tokens[i++]; }
    }

    scene.push_back(std::make_unique<Sol>(x,y,r,parseColor(cor)));
}



        //else if(tk == "Cerca"){
            // Exemplo de valores fixos para Cerca
            // x, y, altura, largura, cor, inclinacao
          //  scene.push_back(std::make_unique<Cerca>(20,5,2,10,"Gray",0));
        }
     /*   } else if(tk == "Arvore"){
            // Exemplo de valores fixos para Arvore
            // x, y, altura, largura, corTronco, corFolhas
            scene.push_back(std::make_unique<Arvore>(10,10,5,3,parseColor("Marrom"),parseColor("Green")));
        } else if(tk == "Sol"){
            // Exemplo de valores fixos para Sol
            // x, y, altura, largura, cor
            scene.push_back(std::make_unique<Sol>(35,25,2,2,parseColor("Yellow")));
        } */
        // TODO: Arvore, Cerca, Sol iguaL
}

void App::run(const std::string& csvFile){
    parseCSV(csvFile);
    if(SDL_Init(SDL_INIT_VIDEO)!=0){
        std::cerr<<"SDL erro: "<<SDL_GetError()<<"\n"; return;
    }
    window = SDL_CreateWindow("ProjetoCG", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    T.init(metersW, metersH, width, height);

    while(running){
        SDL_Event e;
        while(SDL_PollEvent(&e)){
            if(e.type==SDL_QUIT) running=false;
        }
        SDL_SetRenderDrawColor(renderer, bg.r,bg.g,bg.b,bg.a);
        SDL_RenderClear(renderer);
        for(auto& obj : scene) obj->draw(renderer, T);
        SDL_RenderPresent(renderer);
    }
}
