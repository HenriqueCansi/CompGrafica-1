#include "App.h"
#include "Casa.h"
#include "Arvore.h"
#include "Cerca.h"
#include "Sol.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

App::App() : window(nullptr), renderer(nullptr), running(true),
    width(800), height(600), metersW(40), metersH(30), bg({0,0,255,255}) {}

App::~App()
{
    if(renderer) SDL_DestroyRenderer(renderer);
    if(window) SDL_DestroyWindow(window);
    SDL_Quit();
}


std::string trim(const std::string& s)
{
    size_t start = 0;
    while (start < s.size() && std::isspace((unsigned char)s[start])) start++;
    size_t end = s.size();
    while (end > start && std::isspace((unsigned char)s[end-1])) end--;
    return s.substr(start, end - start);
}

std::vector<std::string> App::tokenizeCSV(const std::string& filename)
{
    std::ifstream in(filename);
    std::vector<std::string> tokens;
    if(!in)
    {
        std::cerr<<"Erro ao abrir "<<filename<<"\n";
        return tokens;
    }
    std::string line;
    while(std::getline(in,line))
    {
        std::stringstream ss(line);
        std::string tok;
        while(std::getline(ss,tok,';'))
        {
            if(!tok.empty()) tokens.push_back(tok);
        }
    }
    return tokens;
}

void App::parseCSV(const std::string& filename)
{
    auto tokens = tokenizeCSV(filename);
    auto isObject = [&](const std::string &s)->bool
    {
        return s == "Tela" || s == "Casa" || s == "Arvore" || s == "Cerca" || s == "Sol";
    };

    size_t i = 0;
    while (i < tokens.size())
    {
        std::string tk = tokens[i++];

        if (tk == "Tela")
        {
            while (i < tokens.size() && !isObject(tokens[i]))
            {
                std::string param = tokens[i++];
                if ((param == "Resolução" || param == "Resolucao") && i + 1 < tokens.size())
                {
                    width  = std::stoi(tokens[i++]);
                    height = std::stoi(tokens[i++]);
                }
                else if (param == "Metros" && i + 1 < tokens.size())
                {
                    metersW = std::stod(tokens[i++]);
                    metersH = std::stod(tokens[i++]);
                }
                else if (param == "Cor" && i < tokens.size())
                {
                    bg = parseColor(tokens[i++]);
                }
                else
                {
                    if (i < tokens.size()) ++i;
                }
            }
        }
        else if (tk == "Casa")
        {
            double x=0, y=0, alt=0, larg=0, inclinacao=0;
            std::string corParede, corTelhado, corPorta;

            while (i < tokens.size() && !isObject(tokens[i]))
            {
                std::string p = tokens[i++];
                if ((p == "Localização" || p == "Localizacao") && i + 1 < tokens.size())
                {
                    x = std::stod(tokens[i++]);
                    y = std::stod(tokens[i++]);
                }
                else if (p == "Altura" && i < tokens.size())
                {
                    alt = std::stod(tokens[i++]);
                }
                else if (p == "Largura" && i < tokens.size())
                {
                    larg = std::stod(tokens[i++]);
                }
                else if (p == "CorParede" && i < tokens.size())
                {
                    corParede = tokens[i++];
                }
                else if ((p == "CorTelhado" || p=="Telhado" || p=="Cor Telhado") && i < tokens.size())
                {
                    corTelhado = tokens[i++];
                }
                else if (p == "CorPorta" && i < tokens.size())
                {
                    corPorta = tokens[i++];
                }
                else if (p == "Inclinacao" || p == "Inclinação")
                {
                    if (i < tokens.size()) inclinacao = std::stod(tokens[i++]);
                }
                else {}
            }

            std::cout << "[PARSE] Casa -> x="<<x<<" y="<<y<<" alt="<<alt<<" larg="<<larg
                      <<" corParede="<<corParede<<" corTelhado="<<corTelhado<<" corPorta="<<corPorta <<
                      " inclinacao="<<inclinacao <<  std::endl;

            scene.push_back(std::make_unique<Casa>(
                                x,y,alt,larg,
                                parseColor(corParede),
                                parseColor(corTelhado),
                                parseColor(corPorta),
                                inclinacao
                            ));
        }
        else if (tk == "Arvore")
        {
            double x=0, y=0, alt=0, inclinacao=0;
            std::string corTronco="Marrom", corFolha="Verde";

            while (i < tokens.size() && !isObject(tokens[i]))
            {
                std::string p = tokens[i++];
                p = trim(p);
                if ((p == "Localização" || p == "Localizacao") && i + 1 < tokens.size())
                {
                    x = std::stod(tokens[i++]);
                    y = std::stod(tokens[i++]);
                }
                else if (p == "Altura" && i < tokens.size())
                {
                    alt = std::stod(tokens[i++]);
                }
                else if (p == "CorTronco" || p=="Tronco")
                {
                    if (i < tokens.size()) corTronco = trim(tokens[i++]);
                }
                else if (p == "CorFolha" || p=="Folha" || p=="CorFolhas")
                {
                    if (i < tokens.size()) corFolha = trim(tokens[i++]);
                }
                else if (p == "Inclinacao" || p == "Inclinação")
                {
                    if (i < tokens.size()) inclinacao = std::stod(tokens[i++]);
                }
            }

            std::cout << "[PARSE] Arvore -> x="<<x<<" y="<<y<<" alt="<<alt
                      <<" corTronco="<<corTronco<<" corFolha="<<corFolha
                      <<" inclinacao="<<inclinacao << std::endl;

            scene.push_back(std::make_unique<Arvore>(
                                x, y, alt,
                                parseColor(corTronco),
                                parseColor(corFolha),
                                inclinacao
                            ));

        }
        else if (tk == "Cerca")
        {
            double x=0, y=0, larg=0, alt=0, inclinacao=0;
            std::string cor;

            while (i < tokens.size() && !isObject(tokens[i]))
            {
                std::string p = tokens[i++];
                if ((p == "Localização" || p == "Localizacao") && i + 1 < tokens.size())
                {
                    x = std::stod(tokens[i++]);
                    y = std::stod(tokens[i++]);
                }
                else if (p == "Largura" && i < tokens.size())
                {
                    larg = std::stod(tokens[i++]);
                }
                else if (p == "Altura" && i < tokens.size())
                {
                    alt = std::stod(tokens[i++]);
                }
                else if (p == "Cor" && i < tokens.size())
                {
                    cor = tokens[i++];
                }
                else if (p == "Inclinacao" || p == "Inclinação")
                {
                    if (i < tokens.size()) inclinacao = std::stod(tokens[i++]);
                }
                else {}
            }

            std::cout << "[PARSE] Cerca -> x="<<x<<" y="<<y<<" larg="<<larg<<" alt="<<alt<<" cor="<<cor <<" inclinacao="<<inclinacao<< std::endl;

            scene.push_back(std::make_unique<Cerca>(
                                x,y,larg,alt, parseColor(cor) ,inclinacao
                            ));
        }
        else if (tk == "Sol")
        {
            double x=0, y=0, r=0, inclinacao=0;
            std::string cor="Amarelo";

            while (i < tokens.size() && !isObject(tokens[i]))
            {
                std::string p = trim(tokens[i++]);
                if ((p == "Localização" || p == "Localizacao") && i + 1 < tokens.size())
                {
                    x = std::stod(tokens[i++]);
                    y = std::stod(tokens[i++]);
                }
                else if (p == "Altura" || p=="Altura")
                {
                    if (i < tokens.size()) r = std::stod(tokens[i++]);
                }
                else if (p == "Cor")
                {
                    if (i < tokens.size()) cor = trim(tokens[i++]);
                }
                else if (p == "Inclinacao" || p == "Inclinação")
                {
                    if (i < tokens.size()) inclinacao = std::stod(tokens[i++]);
                }
            }

            std::cout << "[PARSE] Sol -> x="<<x<<" y="<<y<<" raio="<<r<<" cor="<<cor <<" inclinacao="<<inclinacao << std::endl;

            scene.push_back(std::make_unique<Sol>(x,y,r,parseColor(cor),inclinacao));
        }
        else
        {
            std::cout << "[PARSE] token desconhecido: " << tk << std::endl;
        }
    }

    std::cout << "[PARSE] total objects na cena = " << scene.size() << std::endl;
}

void App::run(const std::string& csvFile)
{
    parseCSV(csvFile);
    if(SDL_Init(SDL_INIT_VIDEO)!=0)
    {
        std::cerr<<"SDL erro: "<<SDL_GetError()<<"\n";
        return;
    }
    window = SDL_CreateWindow("ProjetoCG", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    T.init(metersW, metersH, width, height);

    while(running)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            if(e.type==SDL_QUIT) running=false;
        }
        SDL_SetRenderDrawColor(renderer, bg.r,bg.g,bg.b,bg.a);
        SDL_RenderClear(renderer);
        for(auto& obj : scene) obj->draw(renderer, T);
        SDL_RenderPresent(renderer);
    }

}
