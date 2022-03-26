#include "Text.hpp"

Text::Text(std::string ttext, int tsize, SDL_Color tcolor, std::string tfontpath, int tx, int ty, bool centered, SDL_Renderer* trenderer)
	: text(ttext), size(tsize), color(tcolor), fontpath(tfontpath), x(tx), y(ty), renderer(trenderer)
{
	font = TTF_OpenFont(fontpath.c_str(), size);
	textSurf = TTF_RenderText_Solid(font, text.c_str(), color);
	textTex = SDL_CreateTextureFromSurface(renderer, textSurf);
	SDL_FreeSurface(textSurf);
	SDL_QueryTexture(textTex, nullptr, nullptr, &textRect.w, &textRect.h);
	if (centered)
		textRect.x = x - textRect.w / 2;
	else
		textRect.x = x;
	textRect.y = y;
}

void Text::render()
{
	SDL_RenderCopy(renderer, textTex, nullptr, &textRect);
}

void Text::update()
{
	font = TTF_OpenFont(fontpath.c_str(), size);
	textSurf = TTF_RenderText_Solid(font, text.c_str(), color);
	textTex = SDL_CreateTextureFromSurface(renderer, textSurf);
	SDL_FreeSurface(textSurf);
	SDL_QueryTexture(textTex, nullptr, nullptr, &textRect.w, &textRect.h);
	textRect.x = x - textRect.w / 2;
	textRect.y = y;
}
