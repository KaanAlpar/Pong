#include "GameSDL.h"

int processEvents(Game* game)
{
	SDL_Event evnt;
	int done = 0;

	while (SDL_PollEvent(&evnt))
	{
		switch (evnt.type)
		{
		case SDL_WINDOWEVENT_CLOSE:
		{
			if (game->window)
			{
				SDL_DestroyWindow(game->window);
				game->window = NULL;
				done = 1;
			}
		}
		break;
		case SDL_KEYDOWN:
		{
			switch (evnt.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				done = 1;
				break;
			}
		}
		break;
		case SDL_QUIT:
			done = 1;
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	short paddleSpeed = 5;
	//Paddle 1 movement
	if (state[SDL_SCANCODE_W])
	{
		game->paddles[0].y -= paddleSpeed;
	}
	if (state[SDL_SCANCODE_S])
	{
		game->paddles[0].y += paddleSpeed;
	}
	//Paddle 2 movement
	if (state[SDL_SCANCODE_UP])
	{
		game->paddles[1].y -= paddleSpeed;
	}
	if (state[SDL_SCANCODE_DOWN])
	{
		game->paddles[1].y += paddleSpeed;
	}

	return done;
}

void render(Game* game)
{
	Paddle pad1 = game->paddles[0];
	Paddle pad2 = game->paddles[1];
	Ball ball = game->ball;

	SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
	SDL_RenderClear(game->renderer);

	SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);

	SDL_Rect padRec1 = { pad1.x, pad1.y, pad1.w, pad1.h };
	SDL_RenderFillRect(game->renderer, &padRec1);

	SDL_Rect padRec2 = { pad2.x, pad2.y, pad2.w, pad2.h };
	SDL_RenderFillRect(game->renderer, &padRec2);

	SDL_Rect ballRec = { ball.x, ball.y, ball.w, ball.h };
	SDL_RenderFillRect(game->renderer, &ballRec);

	//Render score
	SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
	SDL_Rect scoreRect = { 512 - game->labelW/2, 10,  game->labelW, game->labelH };
	SDL_RenderCopy(game->renderer, game->label, NULL, &scoreRect);
	
	SDL_RenderPresent(game->renderer);
}

void gameLoop(Game* game)
{
	int done = 0;

	initGame(game);
	while (!done)
	{
		collisionDetect(game);

		done = processEvents(game);

		process(game);

		render(game);
	}
}

bool initSDL(Game* game)
{
	bool success = true;

	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("Failed to init SDL! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		window = SDL_CreateWindow("Al-Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
		if (window == NULL)
		{
			printf("Could not create window! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (renderer == NULL)
			{
				printf("Could not create renderer! SDL_Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				game->window = window;
				game->renderer = renderer;
			}
		}
	}

	SDL_RenderSetLogicalSize(renderer, 1024, 640);
	SDL_SetWindowInputFocus(window);

	TTF_Init();

	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);

	loadGame(game);
	return success;
}

void loadGame(Game* game)
{
	game->label = NULL;

	game->font = TTF_OpenFont("res/fonts/font.ttf", 48);
	if (!game->font)
	{
		printf("Cannot find font file!\n");
		SDL_Quit();
		exit(1);
	}

	//Load sounds
	game->bounceSound = Mix_LoadWAV("res/sounds/Jump_03.wav");
	if (game->bounceSound != NULL)
	{
		Mix_VolumeChunk(game->bounceSound, 32);
	}
	game->missSound = Mix_LoadWAV("res/sounds/Explosion_03.wav");
	if (game->missSound != NULL)
	{
		Mix_VolumeChunk(game->missSound, 32);
	}
	
}

void destroySDL(Game* game)
{
	if(game->label != NULL)
		SDL_DestroyTexture(game->label);
	TTF_CloseFont(game->font);

	SDL_DestroyWindow(game->window);
	SDL_DestroyRenderer(game->renderer);

	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}