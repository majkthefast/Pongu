//
// Created by Mikołaj on 11.05.2023.
//

#ifndef pong_objects_h
#define pong_objects_h

#include <SDL.h>

struct Paddle {
    SDL_Point pos;
    SDL_Rect sdlRect;
};

struct Ball {
    SDL_Point speed;
    SDL_Rect sdlRect;
};

#endif //pong_objects_h
