#include "Game.h"
#include "GameSDL.h"

int main(int argc, char** argv)
{
	Game game;
	initSDL(&game);
	gameLoop(&game);
	destroySDL(&game);

	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
		"Alert",
		"Thanks for playing.\nMade by Kaan Alpar - 2017",
		NULL);

	printf("hello");
	return 0;
}