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

Player::Player(SDL_Texture *ptex, int px, int py, float pw, float ph)
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
	s_x = x;
	s_y = y;

    currentFrame = 0;

    speed = 200.0f;
	rotSpeed = 2000.0f;
	speedP = speed;
	rotationForce = 0.05f;
	animationSpeed = 0.07f;
	speedX = 0.0f;
	speedY = 0.0f;
    damp = 0.001;
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
	Time += deltaTime / 100.0f;
    // make the current position the last simulated position, 
    // simulated used to prevent light/small tunneling but does not prevent tunneling 
    x = s_x;
    y = s_y;

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

	angle += rotationSpeed * deltaTime;
	rotationSpeed *= pow(damp, deltaTime);
	if (fabsf(rotationSpeed) < 0.1)
	{
		rotationSpeed = 0;
	}

	// add values to simulated position
	s_x += speedX * deltaTime;
	s_y += speedY * deltaTime;
	rect.x = x;
	rect.y = y;
}

void Player::render(SDL_Texture* texture, SDL_Renderer* renderer)
{
	SDL_RenderCopyEx(renderer, texture, &srcRect, &rect, angle, NULL, SDL_FLIP_NONE);
}

void Player::resetSpeed()
{
	speed = speedP;
}

void Player::noExplore(int winW, int winH)
{
	// left stop
	if (s_x + rect.w/4 <= 0)
	{
		x = 0 - rect.w/4;
		s_x = x;
		speedX *= -1;
	}

	// right stop
	if (s_x + rect.w - rect.w/4 >= winW)
	{
		x = winW - rect.h + rect.w/4;
		s_x = x;
		speedX *= -1;
	}

	// up stop
	if (s_y + rect.h/4 <= 0)
	{
		y = 0 - rect.h/4;
		s_y = y;
		speedY *= -1;
	}

	// down stop
	if (s_y + rect.h - rect.h/4 >= winH )
	{
		y = winH - rect.h + rect.h/4;
		s_y = y;
		speedY *= -1;
	}
}

void Player::removeHealth(int amount)
{
	alpha -= amount;
}
