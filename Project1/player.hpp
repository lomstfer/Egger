#include <SDL.h>
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
	
	/* the simulated position and the other, 
	used with variables instead of just plr.x
	because plr.x acts wierd (maybe it 
	gets rounded) */
	float s_posX;
	float s_posY;
	float posX;
	float posY;

	// speed calculation things
	float posnow;
	float posdiff;
	float speedo;

	// used to be able of slowing down the rotationspeed to make a smoothing
	float rotationSpeed;

    // used to go to the last frame when up key is released
	bool endFramer;

	int r;
	int g;
	int b;
	float alpha;

	double Time;

    Player(SDL_Texture *ptex, int px, int py, float pw, float ph, float windowScale);

	Player() = default;

    void update(int winW, int winH, double deltaTime, const Uint8 *keys);

	void draw(SDL_Texture* TList [9], SDL_Renderer* renderer);

    void resetSpeed();

	void noExplore(int winW, int winH);

	void removeHealth(int amount);
};