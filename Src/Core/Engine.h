//
// Created by Mikołaj on 11.05.2023.
//

#ifndef pong_engine_h
#define pong_engine_h

#include "SDL.h"
#include "SDL_ttf.h"

class Engine {
public:
    Engine();

    ~Engine();

    void init(const char *title, int xPos, int yPos, int width, int height, bool fullscreen);

    void update();

    void render();

    void clean();

    void handleEvents();

    bool windowShouldClose{};
private:
    SDL_Window *window{};
    SDL_Renderer *renderer{};
};


#endif //pong_engine_H
