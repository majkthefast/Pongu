//
// Created by Mikołaj on 11.05.2023.
//

#include "SDL.h"
#include "Core/Engine.h"

#define FPS 60

Engine *game = nullptr;

int main(int argc, char *argv[])
{
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	unsigned int frameTime;

	game = new Engine();

	game->init("Pong", 400, 200, 1280, 720, false); // 1280x720

	while (!game->windowShouldClose)
	{
		frameStart = SDL_GetTicks();

		game->update();
		game->handleEvents();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime)
			SDL_Delay(frameDelay - frameTime);
	}
	game->clean();
	delete game;
	SDL_Quit();
	return 0;
}