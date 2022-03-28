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
	srcRect.w = 8;
	srcRect.h = 12;

    rect.x = ftint(x);
    rect.y = ftint(y);
    rect.w = ftint(w);
    rect.h = ftint(h);

	scale = 0;

	// the simulated position
	s_x = x;
	s_y = y;

    currentFrame = 0;
	animationSpeed = 10.0f;

    speed = 300.0f;
	rotSpeed = 1500.0f;
	speedP = speed;
	rotationForce = 0.05f;
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

	if (currentFrame > 7)
	{
		currentFrame = 0;
	}

	// slow down x speed
	speedX *= pow(damp, deltaTime);
	if (fabsf(speedX) < 0.1)
	{
		speedX = 0;
	}

	// slow down y speed
	speedY *= pow(damp, deltaTime);
	if (fabsf(speedY) < 0.1)
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

void Player::render(SDL_Texture* textureList [8], SDL_Renderer* renderer)
{
	tex = textureList[ftint(currentFrame)];
	SDL_RenderCopyEx(renderer, tex, &srcRect, &rect, angle, NULL, SDL_FLIP_NONE);
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
		s_x = 0 - rect.w/4;
	}

	// right stop
	if (s_x + rect.w - rect.w/4 >= winW)
	{
		s_x = winW - rect.w + rect.w/4;
	}

	// up stop
	if (s_y + rect.h/4 <= 0)
	{
		s_y = 0 - rect.h/4;
	}

	// down stop
	if (s_y + rect.h - rect.h/4 >= winH )
	{
		s_y = winH - rect.h + rect.h/4;
	}
}
