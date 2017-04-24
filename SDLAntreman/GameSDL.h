#pragma once

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <stdio.h>

#include "Game.h"

const int WIDTH = 1024;
const int HEIGHT = 640;

bool initSDL(Game* game);
void destroySDL(Game* game);
void gameLoop(Game* game);
int processEvents(Game* game);
void render(Game* game);
void loadGame(Game* game);