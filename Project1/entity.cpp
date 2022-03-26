#include "Entity.hpp"
#include "Ftint.hpp"
#include <stdlib.h>
#include <math.h>


Entity::Entity(SDL_Texture* texture, int source_width, int source_height, float position_x, float position_y, int width, int height, bool centered)
{
    center = centered;
    tex = texture;

    x = position_x;
    y = position_y;
    w = width;
    h = height;
    if (center)
    {
        x -= w / 2;
        y -= h / 2;
    }
    cx = x + w / 2;
    cy = y + h / 2;
    
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = source_width;
    srcRect.h = source_height;

    rect.x = ftint(x);
    rect.y = ftint(y);
    rect.w = ftint(w);
    rect.h = ftint(h);
}

void Entity::draw(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, tex, &srcRect, &rect);
}

void Entity::update()
{

}

void Entity::moveUpdate()
{
    if (center)
    {
        rect.x = ftint(x);
        rect.y = ftint(y);
        cx = x + w / 2;
        cy = y + h / 2;
    }
    else
    {
        rect.x = ftint(x);
        rect.y = ftint(y);
    }
}