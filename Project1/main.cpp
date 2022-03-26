#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <algorithm>

#include "Entity.hpp"
#include "Text.hpp"

#define Log(x) std::cout << x << std::endl;

void ifquit(bool &level, bool &gameRunning, SDL_Event &event, SDL_Window* window)
{
	if (event.type == SDL_QUIT)
	{
		gameRunning = false;
		level = false;
		// idk, kills the game
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	if (event.key.keysym.sym == SDLK_ESCAPE)
	{
		gameRunning = false;
		level = false;
		// idk, kills the game
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
}

bool collideCenter(SDL_Rect obj, SDL_Rect cobj)
{
	if (cobj.x + cobj.w/2 > obj.x - cobj.w/4 &&
		cobj.y + cobj.h/2 > obj.y - cobj.w/4 &&
		cobj.x + cobj.w/2 < obj.x + obj.w + cobj.w/4 &&
		cobj.y + cobj.h/2 < obj.y + obj.h + cobj.w/4)
	{
		return true;
	}
	else
		return false;
}

int main(int argc, char* args[])
{
	bool gameRunning = true;
	SDL_Event event;

	// screensize
	int winW = 1280;
	int winH = 720;

	// deltaTime, used to make some things not go too fast because you tried to pay to win (ran the program fast)
	Uint64 deltaNow = SDL_GetPerformanceCounter();
	Uint64 deltaLast = 0;
	double deltaTime = 0;

	// init SDL
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();

	SDL_Window* window = SDL_CreateWindow("Egger", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, winW, winH, SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_Surface *icon = IMG_Load("assets/icon.png");
	SDL_SetWindowIcon(window, icon);
	SDL_ShowCursor(SDL_DISABLE);
	SDL_RenderSetLogicalSize(renderer, winW, winH);

	int r = 50;
	int g = 50;
	int b = 50;
	SDL_SetRenderDrawColor(renderer, r, g, b, 0);

	// keyboard state variable
	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	bool menu = true;
	Text titleText("EGGER", 100, {255, 255, 255, 255}, "assets/CascadiaCode.ttf", winW / 2, 30, true, renderer);

	Text enterText("ENTER TO START", 
				 50, {255, 255, 255, 255}, "assets/CascadiaCode.ttf", winW / 2, winH / 2, true, renderer);
	Text fullScreenText("FULLSCREEN ON", 
				 50, {255, 255, 255, 255}, "assets/CascadiaCode.ttf", winW / 2, winH / 2 - 40, true, renderer);
	Text pressFText("press F to change between windowed and fullscreen", 
				 30, {255, 255, 255, 255}, "assets/CascadiaCode.ttf", winW / 2, winH - 100, true, renderer);

	int fullscreenMode = -1;

	bool gameLoad = false;
	bool game = false;
	SDL_Texture* eggTexture = IMG_LoadTexture(renderer, "assets/egg.png");
	Entity egg(eggTexture, 16, 16, winW/2, winH/2, 64, 64, true);

	SDL_Texture* enemyTexture = IMG_LoadTexture(renderer, "assets/enemy.png");
	Entity enemy(enemyTexture, 8, 8, winW/2 + 300, winH/2 - 100, 16, 16, true);
	float enemyEggDistance = 0;
	float eggXdist = 0;
	float eggYdist = 0;
	float enX = 0;
	float enY = 0;

	SDL_Texture* plrTexture = IMG_LoadTexture(renderer, "assets/player.png");
	bool flip = false;
    bool flipv = false;
    float angle = 0;

	SDL_Rect plrSrc;
	plrSrc.x = 0;
	plrSrc.y = 0;
	plrSrc.w = 8;
	plrSrc.h = 8;

	SDL_Rect plr;
	plr.w = 32;
	plr.h = 32;
	plr.x = winW / 2 - plr.w / 2;
	plr.y = winH / 2 - 200 - plr.h / 2;
	
	int speed = 1;
	float damp = 0.96;
	float speedX = 0;
	float speedY = 0;
	float colSpeed = 1;
	
	float plrX = plr.x;
	float plrY = plr.y;
	float s_plrX = plr.x;
	float s_plrY = plr.y;


	// game loop
	while (gameRunning)
	{
		while (menu)
		{
			deltaLast = deltaNow;
			deltaNow = SDL_GetPerformanceCounter();
			deltaTime = (double)(deltaNow - deltaLast) * 100 / (double)SDL_GetPerformanceFrequency();
			while (SDL_PollEvent(&event))
			{
				ifquit(menu, gameRunning, event, window);
				if (event.type == SDL_KEYDOWN)
				{
					switch (event.key.keysym.sym)
					{
					case SDLK_f:
						fullscreenMode *= -1;
						break;
					}
				}
			}
			
			if (fullscreenMode == -1)
			{
				fullScreenText.text = "FULLSCREEN ON";
				SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
				SDL_RenderSetLogicalSize(renderer, winW, winH);
			}
			if (fullscreenMode == 1)
			{
				fullScreenText.text = "FULLSCREEN OFF";
				SDL_SetWindowFullscreen(window, SDL_WINDOW_SHOWN);
				SDL_RenderSetLogicalSize(renderer, winW, winH);
			}
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);
			titleText.render();
			enterText.render();
			pressFText.render();
			fullScreenText.update();
			fullScreenText.render();

			if (keys[SDL_SCANCODE_RETURN])
			{
				menu = false;
				game = true;
				r = 71;
				g = 209;
				b = 103;

				enemy.x = rand() % winW;
				enemy.y = rand() % winH;
				s_plrX = winW / 2 - plr.w / 2;
				s_plrY = winH / 2 - 200 - plr.h / 2;
				speedX = 0;
				speedY = 0;
			}

			// presents everything
			SDL_RenderPresent(renderer);
		}

		while (game)
		{
			deltaLast = deltaNow;
			deltaNow = SDL_GetPerformanceCounter();
			deltaTime = (double)(deltaNow - deltaLast) * 100 / (double)SDL_GetPerformanceFrequency();
			while (SDL_PollEvent(&event))
			{
				ifquit(game, gameRunning, event, window);
			}

			const Uint8 *keys = SDL_GetKeyboardState(NULL);

			{plrX = s_plrX;
			plrY = s_plrY;
			{
			// Events
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
					gameRunning = false;
			}

			if (keys[SDL_SCANCODE_UP])
			{
				speedY = -speed;
				angle = 0;
			}

			if (keys[SDL_SCANCODE_DOWN])
			{
				speedY = speed;
				angle = 180;
			}
			
			if (keys[SDL_SCANCODE_LEFT])
			{
				speedX = -speed;
				flip = false;
				angle = 270;
			}

			if (keys[SDL_SCANCODE_RIGHT])
			{
				speedX = speed;
				flip = true;
				angle = 90;
			}

			if (keys[SDL_SCANCODE_UP] && keys[SDL_SCANCODE_LEFT])
			{
				speedY = -speed * (1/sqrt(2));
				speedX = -speed * (1/sqrt(2));
				angle = 315;
			}

			if (keys[SDL_SCANCODE_UP] && keys[SDL_SCANCODE_RIGHT])
			{
				speedY = -speed * (1/sqrt(2));
				speedX = speed * (1/sqrt(2));
				angle = 45;
			}

			if (keys[SDL_SCANCODE_DOWN] && keys[SDL_SCANCODE_LEFT])
			{
				speedY = speed * (1/sqrt(2));
				speedX = -speed * (1/sqrt(2));
				angle = 225;
			}

			if (keys[SDL_SCANCODE_DOWN] && keys[SDL_SCANCODE_RIGHT])
			{
				speedY = speed * (1/sqrt(2));
				speedX = speed * (1/sqrt(2));
				angle = 135;
			}
			
			// left stop
			if (s_plrX <= 0)
			{
				plrX = 0;
				s_plrX = plrX;
			}

			// right stop
			if (s_plrX + plr.w >= winW)
			{
				plrX = winW - plr.h;
				s_plrX = plrX;
			}

			// up stop
			if (s_plrY <= 0)
			{
				plrY = 0;
				s_plrY = plrY;

			}
			// down stop
			if (s_plrY >= winH - plr.h)
			{
				plrY = winH - plr.h;
				s_plrY = plrY;
			}

			// slow down x speed
			speedX *= pow(damp, deltaTime);
			if (abs(speedX) < 0.1)
			{
				speedX = 0;
			}

			// slow down y speed
			speedY *= pow(damp, deltaTime);
			if (abs(speedY) < 0.1)
			{
				speedY = 0;
			}
			}
			// add values to simulated position
			s_plrX += speedX * deltaTime;
			s_plrY += speedY * deltaTime;
			plr.x = plrX;
			plr.y = plrY;}

			while (s_plrX + plr.w > egg.x + 10 &&
				s_plrX < egg.x + egg.w - 10 &&
				s_plrY + plr.h > egg.y + 10 &&
				s_plrY < egg.y + egg.h - 10)
			{
				if (plrX > egg.x + egg.w - 10)
				{
					s_plrX += colSpeed;
				}
				
				else if (plrX < egg.x + 10)
				{
					s_plrX -= colSpeed;
				}
				
				if (plrY > egg.y + egg.h - 10)
				{
					s_plrY += colSpeed;
				}

				else if (plrY < egg.y + 10)
				{
					s_plrY -= colSpeed;
				}
			}

			Log(enemy.cx);
			eggXdist = enemy.cx - egg.cx;
			eggYdist = enemy.cy - egg.cy;
			enemyEggDistance = sqrt(pow(eggXdist, 2) + pow(eggYdist, 2));
			enX = eggXdist / enemyEggDistance;
			enY = eggYdist / enemyEggDistance;
			enemy.x -= enX * deltaTime * 1;
			enemy.y -= enY * deltaTime * 1;
			if (enemy.cx > egg.x &&
				enemy.cx < egg.x + egg.w &&
				enemy.cy > egg.y &&
				enemy.cy < egg.y + egg.h)
				{
					game = false;
					menu = true;
				}
			enemy.moveUpdate();

			SDL_SetRenderDrawColor(renderer, r, g, b, 255);
			SDL_RenderClear(renderer);
			
			
			egg.draw(renderer);
			enemy.draw(renderer);
			if (flip)
				SDL_RenderCopyEx(renderer, plrTexture, &plrSrc, &plr, 0, NULL, SDL_FLIP_HORIZONTAL);
			if (!flip)
				SDL_RenderCopy(renderer, plrTexture, &plrSrc, &plr);
			/* if (flipv)
				SDL_RenderCopyEx(renderer, plrTexture, &plrSrc, &plr, 0, NULL, SDL_FLIP_VERTICAL);
			if (!flipv)
				SDL_RenderCopy(renderer, plrTexture, &plrSrc, &plr);*/
			SDL_RenderPresent(renderer);
			}
	}	
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}