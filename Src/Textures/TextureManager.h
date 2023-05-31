//
// Created by Mikołaj on 11.05.2023.
//

#ifndef pong_texturemanager_h
#define pong_texturemanager_h

#include "SDL.h"
#include "../../Libs/SDL2_ttf-2.20.2/include/SDL_ttf.h"
#include "iostream"

class TextureManager {
public:
	static SDL_Texture* CreateTextureFromText(TTF_Font* font, std::string text, SDL_Color color, SDL_Renderer *renderer);
};


#endif //pong_texturemanager_h
