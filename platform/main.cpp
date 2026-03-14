
#include "SDL3/SDL.h"
//#include "SDL3/SDL_gamepad.h"
#include "SDL3_mixer/SDL_mixer.h"
//#include "SDL3/SDL_main.h"
#include <string>

#include <stdint.h>
#include <stdbool.h>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

constexpr uint32_t WindowWidth = 1920;
constexpr uint32_t WindowHeight = 1080;

// SDL Window and Surfaces!
SDL_Window* g_window 		= nullptr;
SDL_Renderer* g_renderer	= nullptr;
SDL_Surface* g_window_surf 	= nullptr;
SDL_Surface* g_bg_img_surf 	= nullptr;

bool initialise(void);

/// <summary>
/// 
/// Initialises SDL, Creates the Window and Loads an image
/// 
/// </summary>
/// <returns>success flag, true = it worked!</returns>
bool initialise(void)
{
	bool result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD | SDL_INIT_AUDIO);
	if(!result)
	{
		SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
	}
	else
	{
		// Do stuff
		result = SDL_CreateWindowAndRenderer("Freeze!", WindowWidth, WindowHeight, SDL_WINDOW_FULLSCREEN, &g_window, &g_renderer);
		if(!result)
		{
			SDL_Log("SDL failed to create window and renderer! SDL error: %s\n", SDL_GetError());
		}
		else 
		{
			SDL_GetWindowSurface(g_window);
			result = MIX_Init();
			if(!result)
			{
				SDL_Log("SDL failed to initialise Audio! SDL_mixer error: %s\n", SDL_GetError());
			}
		}
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

		SDL_Color clear_colour =
		{
			.r = 200, .g = 200, .b = 255, .a = 255
		};

		SDL_Gamepad * gamepad = NULL;
		int gamepad_count = 0;
		SDL_JoystickID * gpid = SDL_GetGamepads(&gamepad_count);

		if(gamepad_count && (nullptr != gpid))
		{
			gamepad = SDL_OpenGamepad(*gpid);
		}

		bool running = true;
		SDL_Event evt;
		while (running)
		{
			while (SDL_PollEvent(&evt))
			{
				if(evt.type == SDL_EVENT_GAMEPAD_REMOVED)
				{
					if(NULL != gamepad)
					{
						SDL_CloseGamepad(gamepad);
					}
				}
				else if(evt.type == SDL_EVENT_GAMEPAD_ADDED)
				{
					if(NULL == gamepad)
					{
						gpid = SDL_GetGamepads(&gamepad_count);
						gamepad = SDL_OpenGamepad(*gpid);
					}
				}

				if(NULL != gamepad) // Gamepad controls if available
				{
					if(evt.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN)
					{
						switch (evt.gbutton.button)
						{
							case SDL_GAMEPAD_BUTTON_BACK:
								running = false;
								break;
							
							default:
								break;
						}
						
					}

					if(evt.type == SDL_EVENT_GAMEPAD_BUTTON_UP)
					{
						
					}
				}
			}
			if(running)
			{
				SDL_SetRenderDrawColor(g_renderer, clear_colour.r, clear_colour.g, clear_colour.b, clear_colour.a);
				SDL_RenderClear(g_renderer);

				// SDL_FillSurfaceRect(g_window_surf, NULL, SDL_MapRGBA(SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_ABGR8888), NULL, 200, 200, 240, 255));
				// SDL_RenderTexture(g_renderer, SDL_CreateTextureFromSurface(g_renderer, g_window_surf), NULL, NULL);
				// do something
				SDL_RenderPresent(g_renderer);
			}
		}

		SDL_Quit();
	}

	std::cout << "..Freeze done." << std::endl;
	return 0;
}
