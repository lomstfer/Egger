#include "Player.hpp"

/* Player::Entity(SDL_Texture* etex, int srcW, int srcH, float ex, float ey, int ew, int eh)
{
	tex = etex;

    x = ex;
    y = ey;
    w = ew;
    h = eh;
    
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = srcW;
    srcRect.h = srcH;

    dstRect.x = x;
    dstRect.y = y;
    dstRect.w = w;
    dstRect.h = h;
} */

Player::Player(SDL_Texture *ptex, int px, int py, float pw, float ph, float windowScale)
{
    tex = ptex;
    w = pw;
    h = ph;
    x = px - w / 2;
    y = py - h / 2;
	
	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = 10;
	srcRect.h = 10;

    rect.x = ftint(x);
    rect.y = ftint(y);
    rect.w = ftint(w);
    rect.h = ftint(h);

	scale = 0;

	// the simulated position 
	s_posX = x;
	s_posY = y;

    currentFrame = 0;

    speed = 0.15f * windowScale;
	speedP = speed;
	rotationForce = 0.05f;
	animationSpeed = 0.07f;
	speedX = 0.0f;
	speedY = 0.0f;
    damp = 0.98f;
    angle = 0.0f;
	angleRadians = angle * M_PI / 180;

	// used to be able of slowing down the rotationspeed to make a smoothing
	rotationSpeed = 0.0f;

    // used to go to the last frame when up key is released
	endFramer = false;

	r = 255;
	g = 255;
	b = 255;
	alpha = 255;

	Time = 0;
}

void Player::update(int winW, int winH, double deltaTime, const Uint8 *keys)
{

}

void Player::draw(SDL_Texture* TList [9], SDL_Renderer* renderer)
{
	tex = TList[int(currentFrame)];
	SDL_SetTextureAlphaMod(tex, alpha);
	SDL_RenderCopyEx(renderer, tex, &srcRect, &rect, angle, NULL, SDL_FLIP_NONE);
}

void Player::resetSpeed()
{
	speed = speedP;
}

void Player::noExplore(int winW, int winH)
{
	// left stop
	if (s_posX + rect.w/4 <= 0)
	{
		x = 0 - rect.w/4;
		s_posX = x;
		speedX *= -1;
	}

	// right stop
	if (s_posX + rect.w - rect.w/4 >= winW)
	{
		x = winW - rect.h + rect.w/4;
		s_posX = x;
		speedX *= -1;
	}

	// up stop
	if (s_posY + rect.h/4 <= 0)
	{
		y = 0 - rect.h/4;
		s_posY = y;
		speedY *= -1;
	}

	// down stop
	if (s_posY + rect.h - rect.h/4 >= winH )
	{
		y = winH - rect.h + rect.h/4;
		s_posY = y;
		speedY *= -1;
	}
}

void Player::removeHealth(int amount)
{
	alpha -= amount;
}
