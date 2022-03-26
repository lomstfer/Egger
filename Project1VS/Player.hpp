#include <SDL2/SDL.h>
#include <math.h>
#include "Ftint.hpp"

//#include "Entity.hpp"

class Player// : public Entity
{
public:
	//using Entity::Entity;
    SDL_Texture* tex;
    float x;
    float y;
    float w;
    float h;
	float s_x;
	float s_y;

	SDL_Rect srcRect;
    SDL_Rect rect;

	float scale;

    float currentFrame;

    float speed;
	float speedP;
	float rotationForce;
	float animationSpeed;
	float speedX;
	float speedY;
    float damp;

    float angle;
	float angleRadians;

	// speed calculation things
	float posnow;
	float posdiff;
	float speedo;

	// used to be able of slowing down the rotationspeed to make a smoothing
	float rotSpeed;
	float rotationSpeed;

    // used to go to the last frame when up key is released
	bool endFramer;

	int r;
	int g;
	int b;
	float alpha;

	double Time;

    Player(SDL_Texture *ptex, int px, int py, float pw, float ph);

	Player() = default;

    void update(int winW, int winH, double deltaTime, const Uint8 *keys);

	void render(SDL_Texture* textureList [8], SDL_Renderer* renderer);

    void resetSpeed();

	void noExplore(int winW, int winH);
};