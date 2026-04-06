
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

std::string font_path = "./assets/fonts/Passero_One/Passero_One-Regular.ttf";

// SDL Window and Surfaces!
SDL_Window* g_window 		= nullptr;
SDL_Renderer* g_renderer	= nullptr;
SDL_Surface* g_window_surf 	= nullptr;
SDL_Surface* g_bg_img_surf 	= nullptr;

struct FontData
{
    uint8_t *data = NULL;
    size_t  size  = 0;
} g_freeze_font;

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

    // Load Font File
    g_freeze_font.data = (uint8_t *)SDL_LoadFile(font_path.c_str(), &g_freeze_font.size);
    if(NULL !=  g_freeze_font.data)
    {
        std::cout << "FONT [" << font_path << "] size:=" << g_freeze_font.size << std::endl;
        std::cout << "     [" << font_path << "] data[0]:=" << g_freeze_font.data[0] << std::endl;;
        std::cout << "Magic me get on so well" << std::endl;
    }
    else
    {
        std::cout << "Font loading failed" << std::endl;
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
                else
                {
                    const bool* state = SDL_GetKeyboardState(NULL);
                    
                    if (state[SDL_SCANCODE_ESCAPE] || state[SDL_SCANCODE_Q]) 
                    {
                        running = false;
                    }
                }
            }
            if(running)
            {
                SDL_SetRenderDrawColor(g_renderer, clear_colour.r, clear_colour.g, clear_colour.b, clear_colour.a);
                SDL_RenderClear(g_renderer);

                // do something
                SDL_RenderPresent(g_renderer);
            }
        }

        SDL_Quit();
    }

    std::cout << "..Freeze done." << std::endl;
    return 0;
}
