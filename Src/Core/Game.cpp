//
// Created by Mikołaj on 11.05.2023.
//

#include "Game.h"

bool Game::SetDirection() {
    int randomValue = rand() % 10 + 1; //random ball direciton 1-10

    return randomValue > 5;
}