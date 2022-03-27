#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <math.h>

#include "Player.hpp"
#include "Entity.hpp"
#include "Text.hpp"
#include "Ftint.hpp"


#define Log(x) std::cout << x << std::endl;

void ifquit(bool &game, bool &gameRunning, SDL_Event &event, SDL_Window* window)
{
	if (event.type == SDL_QUIT)
	{
		gameRunning = false;
		game = false;
	}

	if (event.key.keysym.sym == SDLK_ESCAPE)
	{
		gameRunning = false;
		game = false;
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

	Uint64 inputNow = SDL_GetPerformanceCounter();
	Uint64 inputLast = 0;
	double inputTime = 0;

	Uint64 updateNow = SDL_GetPerformanceCounter();
	Uint64 updateLast = 0;
	double updateTime = 0;

	Uint64 renderNow = SDL_GetPerformanceCounter();
	Uint64 renderLast = 0;
	double renderTime = 0;

	int updateElementLogs = 0;

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
	Text titleText("EGGER", 100, {255, 255, 255, 255}, "assets/CascadiaCode.ttf", winW / 2, winH/2 - 200, true, renderer);

	Text enterText("ENTER TO START - ESCAPE TO EXIT", 
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
	std::vector<Entity> enemies;
	float enemyEggDistance = 0;
	float eggXdist = 0;
	float eggYdist = 0;
	float enSpeedX = 0;
	float enSpeedY = 0;
	float enSpeedMulti = 1;
	float enemySpawnTime = 0;
	int enemyRandomSpawnTime = rand() % 3 + 2;
	int sideX = rand() % 3 - 1;
	int sideY = rand() % 3 - 1;
	float enLastX = 0;
	float enLastY = 0;

	SDL_Texture* henTex0 = IMG_LoadTexture(renderer, "assets/hen0.png");
	SDL_Texture* henTex1 = IMG_LoadTexture(renderer, "assets/hen1.png");
	SDL_Texture* henTex2 = IMG_LoadTexture(renderer, "assets/hen2.png");
	SDL_Texture* henTex3 = IMG_LoadTexture(renderer, "assets/hen3.png");
	SDL_Texture* henTex4 = IMG_LoadTexture(renderer, "assets/hen4.png");
	SDL_Texture* henTex5 = IMG_LoadTexture(renderer, "assets/hen5.png");
	SDL_Texture* henTex6 = IMG_LoadTexture(renderer, "assets/hen6.png");
	SDL_Texture* henTex7 = IMG_LoadTexture(renderer, "assets/hen7.png");
	SDL_Texture* henTexList [8] = {henTex0, henTex1, henTex2, henTex3, henTex4, henTex5, henTex6 ,henTex7};
	Player player(henTex0, winW/2, winH/2 + 200, 32, 48);

	float colSpeed = 1;

	float score = 0;
	float highscore = 0;
	float scoreAdder = 1;
	Text scoreText("HIGHSCORE: " + std::to_string(ftint(score)), 
				   50, {255, 255, 255, 255}, "assets/CascadiaCode.ttf", winW/2, 20, true, renderer);

	float gameTime = 0;
	bool win = false;

	// program running
	while (gameRunning)
	{
		// menu loop
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
			}
			if (fullscreenMode == 1)
			{
				fullScreenText.text = "FULLSCREEN OFF";
				SDL_SetWindowFullscreen(window, SDL_WINDOW_SHOWN);
			}
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);
			titleText.render();
			enterText.render();
			pressFText.render();
			fullScreenText.update();
			fullScreenText.render();

			if (ftint(score) > ftint(highscore))
			{
				highscore = score + 1;
				scoreText.text = "NEW HIGHSCORE: " + std::to_string(ftint(highscore - 1));
			}
			else
			{
				scoreText.text = "HIGHSCORE: " + std::to_string(ftint(highscore));
			}
				
			scoreText.color = {255, 255, 255, 255};
			scoreText.update();
			scoreText.render();
			if (keys[SDL_SCANCODE_RETURN])
			{
				menu = false;
				game = true;
				gameTime = 0;
				r = 71;
				g = 209;
				b = 103;
				scoreText.color = {255, 255, 255, 255};
				
				player.s_x = winW / 2 - player.w / 2;
				player.s_y = winH / 2 - 200 - player.h / 2;
				player.speedX = 0;
				player.speedY = 0;
				player.angle = 0;
				player.rotationSpeed = 0;
				score = 0;
				scoreAdder = 1;
				enSpeedMulti = 0;
			}

			// presents everything
			SDL_RenderPresent(renderer);
		}

		// game running
		while (game)
		{
			deltaLast = deltaNow;
			deltaNow = SDL_GetPerformanceCounter();
			deltaTime = (double)(deltaNow - deltaLast) / (double)SDL_GetPerformanceFrequency();
			gameTime += deltaTime;

			// INPUT
			inputLast = SDL_GetTicks();
			while (SDL_PollEvent(&event))
			{
				ifquit(game, gameRunning, event, window);
			}

			if (keys[SDL_SCANCODE_UP])
			{
				player.currentFrame += player.animationSpeed * deltaTime;
				player.angleRadians = player.angle * M_PI / 180;

				player.speedY = -cos(player.angleRadians) * player.speed;
				player.speedX = sin(player.angleRadians) * player.speed;
			}
			else
				player.currentFrame = 0;

			if (keys[SDL_SCANCODE_LEFT])
			{
				player.rotationSpeed += -deltaTime * player.rotSpeed;
			}

			if (keys[SDL_SCANCODE_RIGHT])
			{
				player.rotationSpeed += deltaTime * player.rotSpeed;
			}
			inputNow = SDL_GetTicks();
			inputTime = inputNow - inputLast;

			// UPDATE
			updateLast = SDL_GetTicks();
			while (player.s_x + player.w > egg.x + 10 &&
				player.s_x < egg.x + egg.w - 10 &&
				player.s_y + player.h > egg.y + 10 &&
				player.s_y < egg.y + egg.h - 10)
			{
				if (player.x > egg.x + egg.w - 10)
				{
					player.s_x += colSpeed;
				}
				
				else if (player.x + player.w < egg.x + 10)
				{
					player.s_x -= colSpeed;
				}
				
				if (player.y > egg.y + egg.h - 10)
				{
					player.s_y += colSpeed;
				}

				else if (player.y < egg.y + 10)
				{
					player.s_y -= colSpeed;
				}
			}

			enemySpawnTime += deltaTime;
			sideX = rand() % 3 - 1;
			sideY = rand() % 3 - 1;
			if (enemySpawnTime >= enemyRandomSpawnTime && gameTime < 55)
			{
				enemyRandomSpawnTime = 2;
				enemySpawnTime = 0;
				
				if (sideX != 0 || sideY != 0)
				{
					Entity enemy(enemyTexture, 8, 8, winW/2 + rand() % 100 - 50 + (rand() % 300 + 600) * sideX, winH/2 + rand() % 100 - 50 + (rand() % 300 + 400) * sideY, 16, 16, true);
					enemies.push_back(enemy);
				}
					
			}

			enSpeedMulti += scoreAdder * deltaTime / 10;
			for (unsigned int i = 0; i < enemies.size(); i++)
			{
				eggXdist = enemies[i].cx - egg.cx;
				eggYdist = enemies[i].cy - egg.cy;
				enemyEggDistance = sqrt(pow(eggXdist, 2) + pow(eggYdist, 2));
				enSpeedX = eggXdist / enemyEggDistance;
				enSpeedY = eggYdist / enemyEggDistance;
				enemies[i].x -= enSpeedX * deltaTime * 30 * enSpeedMulti;
				enemies[i].y -= enSpeedY * deltaTime * 30 * enSpeedMulti;
				if (enemies[i].cx > egg.x &&
					enemies[i].cx < egg.x + egg.w &&
					enemies[i].cy > egg.y &&
					enemies[i].cy < egg.y + egg.h)
					{
						enemies.erase(enemies.begin() + i);
						score -= 50;
					}
				if (collideCenter(player.rect, enemies[i].rect))
				{
					enemies.erase(enemies.begin() + i);
					score += 10;
				}
				enemies[i].moveUpdate();
				
			}

			scoreAdder *= 1.0001f;
			score += scoreAdder * deltaTime;

			if (score < 0)
			{
				score = 0;
			}

			scoreText.text = std::to_string(ftint(score));
			scoreText.update();
			
			player.update(winW, winH, deltaTime, keys);
			player.noExplore(winW, winH);

			updateNow = SDL_GetTicks();
			updateTime = updateNow - updateLast;

			if (gameTime >= 60)
			{
				enemies.clear();
				win = true;
				game = false;
				menu = true;
			}

			// RENDER
			renderLast = SDL_GetTicks();
			SDL_SetRenderDrawColor(renderer, r, g, b, 255);
			SDL_RenderClear(renderer);
			for (unsigned int i = 0; i < enemies.size(); i++)
				enemies[i].render(renderer);
			player.render(henTexList, renderer);
			egg.render(renderer);
			scoreText.render();
			SDL_RenderPresent(renderer);

			renderNow = SDL_GetTicks();
			renderTime = renderNow - renderLast;
			updateElementLogs += 1;
			if (updateElementLogs % 10 == 0)
				std::cout << "INPUT: " + std::to_string(ftint(inputTime)) + " | UPDATE: " + std::to_string(ftint(updateTime)) + " | RENDER: " + std::to_string(ftint(renderTime)) + " (TICKS)" << std::endl;
		}
	}
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}