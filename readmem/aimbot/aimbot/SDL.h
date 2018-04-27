#pragma once
#ifndef SDL_H
#define SDL_H

#include <SDL.h>
#include <SDL_image.h>

//Starts up SDL and creates window
bool init();

//Frees media and shuts down SDL
void close();

//The window renderer
extern SDL_Renderer* gRenderer;
//The window we'll be rendering to
extern SDL_Window* gWindow;
#endif