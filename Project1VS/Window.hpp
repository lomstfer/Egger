#pragma once
#include <string>
#include <SDL2/SDL.h>

class Window
{
public:
	Window(const std::string &title, int width, int height);
	~Window();

	void pollEvents();
	void clear(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const;

	inline bool isClosed() const { return _closed; }


private:
	bool init();

private:
	std::string _title;
	int _width = 640;
	int _height = 480;

	bool _closed = false;

	SDL_Window* _window = nullptr;
	SDL_Renderer* _renderer = nullptr;
};