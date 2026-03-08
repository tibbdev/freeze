
#include "SDL3/SDL.h"
//#include "SDL3/SDL_gamepad.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "SDL3_mixer/SDL_mixer.h"
//#include "SDL3/SDL_main.h"
#include <string>

#include <stdint.h>
#include <stdbool.h>

#include <iostream>

constexpr uint32_t WindowWidth = 1920;
constexpr uint32_t WindowHeight = 1080;

// SDL Window and Surfaces!
SDL_Window* window = nullptr;
SDL_Surface* window_surf = nullptr;
SDL_Surface* bg_img_surf = nullptr;

bool initialise(void);

/// <summary>
/// 
/// Initialises SDL, Creates the Window and Loads an image
/// 
/// </summary>
/// <returns>success flag, true = it worked!</returns>
bool initialise(void)
{
	bool result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD);
	if(!result)
	{
		SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
	}
	else
	{
		// Do stuff
	}

	return result;
}

int main()
{
	std::cout << "Freeze Starting..." << std::endl;

	if (!initialise())
	{
		std::cout << " - Initialisation Failed :'(" << std::endl;
	}
	else
	{
		std::cout << " - Initialisation Success!" << std::endl;
		SDL_Quit();
	}

	std::cout << "..Freeze done." << std::endl;
	return 0;
}
