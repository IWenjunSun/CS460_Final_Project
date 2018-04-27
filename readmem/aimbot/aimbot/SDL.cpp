#include "SDL.h"
#include <iostream>
SDL_Renderer* gRenderer = NULL;
SDL_Window* gWindow = NULL;

using namespace std;

/* SDL functions */
bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << endl;
		success = false;
	}
	return success;
}

void close() {

	SDL_Quit();
}