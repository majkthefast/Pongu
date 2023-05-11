//
// Created by Mikołaj on 11.05.2023.
//

#include "TextureManager.h"

SDL_Texture* TextureManager::CreateTextureFromText(TTF_Font* font, std::string text, SDL_Color color, SDL_Renderer *renderer)
{
	SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
	SDL_Texture* texTure = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	return texTure;
}