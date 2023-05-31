//
// Created by Mikołaj on 11.05.2023.
//

#include <SDL_mixer.h>
#include "Engine.h"
#include "../Objects/objects.h"
#include "../Textures/TextureManager.h"
#include "Game.h"

bool play, ballDirection;
int playerOneScore, playerTwoScore;

Paddle playerOne, playerTwo;
Ball boll;
TTF_Font *font = nullptr;
Mix_Chunk *wallHitSound = nullptr;
Mix_Chunk *paddleHitSound = nullptr;
SDL_Rect screenRect, scoreRec1, scoreRec2, winRec;
SDL_Texture *playerOne_score;
SDL_Texture *playerTwo_score;


Engine::Engine() = default;

Engine::~Engine() = default;

void Engine::init(const char *title, int xPos, int yPos, int width, int height, bool fullscreen) {
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    int flags = 0;
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        window = SDL_CreateWindow(title, xPos, yPos, width, height, flags);

        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        } else {
            printf("Fatal error!\n");
            windowShouldClose = true;
        }
    }
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    //Scoreboard
    screenRect = {0, 0, width, height};
    scoreRec1 = {static_cast<int>(width * 0.25), 50, 40, 60}; // x, y, w, h
    scoreRec2 = {static_cast<int>(width * 0.75), 50, 40, 60};

    //P1 Paddle
    playerOne.pos.x = 50;
    playerOne.pos.y = height / 2;
    playerOne.sdlRect.x = playerOne.pos.x;
    playerOne.sdlRect.y = playerOne.pos.y;
    playerOne.sdlRect.w = 10;
    playerOne.sdlRect.h = 100;

    //P2 Paddle
    playerTwo.pos.x = width - 50;
    playerTwo.pos.y = height / 2;
    playerTwo.sdlRect.x = playerTwo.pos.x;
    playerTwo.sdlRect.y = playerTwo.pos.y;
    playerTwo.sdlRect.w = 10;
    playerTwo.sdlRect.h = 100;

    //Ball
    boll.speed = { 10,  10};
    boll.sdlRect.x = 1280 / 2;
    boll.sdlRect.y = 720 / 2;
    boll.sdlRect.w = 20;
    boll.sdlRect.h = 20;

    font = TTF_OpenFont("Assets/pixel.ttf", 50);
    if (!font) {
        printf("Fatal error!\n");
        windowShouldClose = true;
    }

    wallHitSound = Mix_LoadWAV("Assets/WallHit.wav");
    paddleHitSound = Mix_LoadWAV("Assets/PaddleHit.wav");
    if ( !wallHitSound || !paddleHitSound ) {
        printf("Fatal error!\n");
        windowShouldClose = true;
    }

    playerOneScore = 0;
    playerTwoScore = 0;
    play = true;

    playerOne_score = TextureManager::CreateTextureFromText(font, std::to_string(playerOneScore), {255, 255, 255, 255},
                                                     renderer);
    playerTwo_score = TextureManager::CreateTextureFromText(font, std::to_string(playerTwoScore), {255, 255, 255, 255},
                                                     renderer);
}

void Engine::update() {
    //logic:
    if (!play) {
        if (!ballDirection) {
            boll.sdlRect.x -= boll.speed.x;
            boll.sdlRect.y -= boll.speed.y;
        } else {
            boll.sdlRect.x += boll.speed.x;
            boll.sdlRect.y += boll.speed.y;
        }

        // Ball collision with walls:
        if (boll.sdlRect.y < 0)
            boll.speed.y *= -1;
        if (boll.sdlRect.y > screenRect.h - 20)
            boll.speed.y *= -1;

        if (boll.sdlRect.x < -20) { // Player 2 scores
            boll.sdlRect.x = screenRect.w / 2;
            boll.sdlRect.y = screenRect.h / 2;
            playerTwoScore++;
            playerTwo_score = TextureManager::CreateTextureFromText(font, std::to_string(playerTwoScore), {255, 255, 255, 255},
                                                             renderer);
            play = true;

            if(playerTwoScore == 10){
                winRec = {static_cast<int>(screenRect.w * 0.15), static_cast<int>(screenRect.h * 0.25), static_cast<int>(screenRect.w * 0.75), static_cast<int>(screenRect.h * 0.4)};
                SDL_RenderCopy(renderer, TextureManager::CreateTextureFromText(font, "Player Two Wins!", {255, 255, 255, 255}, renderer), nullptr, &winRec);
                SDL_RenderPresent(renderer);
                SDL_Delay(3000);
                windowShouldClose = true;
            }
        }
        if (boll.sdlRect.x > screenRect.w) { // Player 1 scores
            boll.sdlRect.x = screenRect.w / 2;
            boll.sdlRect.y = screenRect.h / 2;
            playerOneScore++;
            playerOne_score = TextureManager::CreateTextureFromText(font, std::to_string(playerOneScore), {255, 255, 255, 255},
                                                             renderer);
            play = true;

            if(playerOneScore == 10){
                winRec = {static_cast<int>(screenRect.w * 0.15), static_cast<int>(screenRect.h * 0.25), static_cast<int>(screenRect.w * 0.75), static_cast<int>(screenRect.h * 0.4)};
                SDL_RenderCopy(renderer, TextureManager::CreateTextureFromText(font, "Player One Wins!", {255, 255, 255, 255}, renderer), nullptr, &winRec);
                SDL_RenderPresent(renderer);
                SDL_Delay(3000);
                windowShouldClose = true;
            }
        }

        if (boll.sdlRect.x == playerOne.sdlRect.x + playerOne.sdlRect.w && boll.sdlRect.y >= playerOne.sdlRect.y - 20 && boll.sdlRect.y <= playerOne.sdlRect.y + 100) { // Player 1
            boll.speed.x *= -1; // Change direction
            int paddleCenter = playerOne.sdlRect.y + playerOne.sdlRect.h / 2; // Środek paletki Player 1
            int ballCenter = boll.sdlRect.y + boll.sdlRect.h / 2; // Środek piłki

            int distanceFromCenter = ballCenter - paddleCenter; // Odległość od środka paletki

            boll.speed.y = distanceFromCenter * 0.2; // Zależnie od miejsca trafienia w paletkę zmień prędkość

            Mix_PlayChannel(-1, paddleHitSound, 0);
        }

        if (boll.sdlRect.x == playerTwo.sdlRect.x - playerTwo.sdlRect.w && boll.sdlRect.y >= playerTwo.sdlRect.y - 20 && boll.sdlRect.y <= playerTwo.sdlRect.y + 100) { // Player 2
            boll.speed.x *= -1; // Change direction
            int paddleCenter = playerTwo.sdlRect.y + playerTwo.sdlRect.h / 2; // Środek paletki Player 2
            int ballCenter = boll.sdlRect.y + boll.sdlRect.h / 2; // Środek piłki

            int distanceFromCenter = ballCenter - paddleCenter; // Odległość od środka paletki

            boll.speed.y = distanceFromCenter * 0.2; // Zależnie od miejsca trafienia w paletkę zmień prędkość

            Mix_PlayChannel(-1, paddleHitSound, 0);
        }

        if (playerOne.pos.y < 0){ // Paddle collision with walls
            playerOne.pos.y = 0;  // Player 1 stop paddle top
            Mix_PlayChannel(-1, wallHitSound, 0);
        }
        if (playerOne.pos.y + playerOne.sdlRect.h > screenRect.h) {
            playerOne.pos.y = screenRect.h - playerOne.sdlRect.h;  // Player 1 stop paddle bottom
            Mix_PlayChannel(-1, wallHitSound, 0);
        }

        if (playerTwo.pos.y < 0) {
            playerTwo.pos.y = 0;    // Player 2 stop paddle top
        }
        if (playerTwo.pos.y + playerTwo.sdlRect.h > screenRect.h) {
            playerTwo.pos.y = screenRect.h - playerTwo.sdlRect.h;   // Player 2 stop paddle bottom
        }


    }
    else {
        playerOne.pos.y = screenRect.h / 2 - 20;  // Reset paddles
        playerTwo.pos.y = screenRect.h / 2 - 20;
        boll.speed = { 10,  10};               // Reset ball speed
    }

    // Update paddle position
    playerOne.sdlRect.x = playerOne.pos.x;
    playerOne.sdlRect.y = playerOne.pos.y;
    playerTwo.sdlRect.x = playerTwo.pos.x;
    playerTwo.sdlRect.y = playerTwo.pos.y;

    render();
}

void Engine::render() {
    // Draw:
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255,255,255, 0.9);
    SDL_RenderFillRect(renderer, &playerOne.sdlRect);

    SDL_RenderFillRect(renderer, &playerTwo.sdlRect);

    SDL_RenderFillRect(renderer, &boll.sdlRect);

    SDL_RenderCopy(renderer, playerOne_score, nullptr, &scoreRec1);
    SDL_RenderCopy(renderer, playerTwo_score, nullptr, &scoreRec2);

    //Net:
    for (int y = 0; y < 720; ++y)
    {
        if (y % 5)
        {
            SDL_RenderDrawPoint(renderer, 1280 / 2, y);
        }
    }
    SDL_RenderPresent(renderer);
}
//void Engine::renderWin() {
//    // Draw:
//    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//    SDL_RenderClear(renderer);
//    SDL_Rect winRec = {screenRect.w / 2 - 100, screenRect.h / 2 - 50, 200, 100};
//    if(playerOneScore == 1){
//        SDL_Texture *playerOne_win = TextureManager::CreateTextureFromText(font, "Player 1 win!", {255, 255, 255, 255}, renderer);
//        SDL_RenderCopy(renderer, playerOne_win, nullptr, &winRec);
//    }
//    else if(playerTwoScore == 10){
//        SDL_Texture *playerTwo_win = TextureManager::CreateTextureFromText(font, "Player 2 win!", {255, 255, 255, 255}, renderer);
//        SDL_RenderCopy(renderer, playerTwo_win, nullptr, &winRec);
//    }


//    SDL_SetRenderDrawColor(renderer, 255,255,255, 0.9);
//
//    SDL_RenderCopy(renderer, playerOne_score, nullptr, &scoreRec1);
//    SDL_RenderCopy(renderer, playerTwo_score, nullptr, &scoreRec2);
//
//    SDL_RenderPresent(renderer);
//}

void Engine::clean() {
    TTF_CloseFont(font);
    SDL_DestroyTexture(playerOne_score);
    SDL_DestroyTexture(playerTwo_score);
    Mix_FreeChunk(wallHitSound);
    Mix_FreeChunk(paddleHitSound);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    Mix_Quit();
    TTF_Quit();
}

void Engine::handleEvents() {

    const Uint8 *state = SDL_GetKeyboardState(nullptr);

    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            windowShouldClose = true;
            break;
        case SDL_KEYDOWN:
            if (play) {
                if (state[SDL_SCANCODE_SPACE]) {
                    ballDirection = Game::SetDirection();
                    play = false;
                }
            }
            break;
        default:
            break;
    }

    if (state[SDL_SCANCODE_UP])
        playerTwo.sdlRect.y = playerTwo.pos.y -= 10;
    if (state[SDL_SCANCODE_DOWN])
        playerTwo.sdlRect.y = playerTwo.pos.y += 10;
    if (state[SDL_SCANCODE_W])
        playerOne.sdlRect.y = playerOne.pos.y -= 10;
    if (state[SDL_SCANCODE_S])
        playerOne.sdlRect.y = playerOne.pos.y += 10;
    if (state[SDL_SCANCODE_ESCAPE]) {
        windowShouldClose = true;
    }
}