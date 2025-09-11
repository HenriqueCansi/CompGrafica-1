#ifndef APP_H
#define APP_H

#include <SDL2/SDL.h>
#include <vector>
#include <memory>
#include <string>
#include "Shape.h"
#include "Color.h"
#include "Transformations.h"

class App {
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
    std::vector<std::unique_ptr<Shape>> scene;
    Transform T;
    Color bg;
    int width, height;
    double metersW, metersH;

    std::vector<std::string> tokenizeCSV(const std::string& filename);
    void parseCSV(const std::string& filename);

public:
    App();
    ~App();
    void run(const std::string& csvFile);
};

#endif
