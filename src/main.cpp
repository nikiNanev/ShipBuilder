#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <SDL3_image/SDL_image.h>

#define WINDOW_WIDTH 1366
#define WINDOW_HEIGHT 768

// Rectangle properties
float x_pos = 0.0f;
float y_pos = 0.0f;

float x_factor = 1.03f;
float y_factor = 1.03f;

uint16_t x_iterations = 0;
uint16_t y_iterations = 0;

int main(int argc, char *argv[])
{

    SDL_Window *window = SDL_CreateWindow(
        "Ship Builder",
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0 // plain window for the SDL renderer
    );

    if (SDL_Init(SDL_INIT_VIDEO) != true)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    if (window == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create a renderer for the window. SDL3 provides SDL_CreateRendererForWindow(window, driverName, flags).
    SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);

    if (renderer == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool done = false;
    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT || event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
            {
                done = true;
            }

            if (event.type == SDL_EVENT_KEY_DOWN)
            {
                SDL_assert(event.type == SDL_EVENT_KEY_DOWN);

                if (event.key.scancode == SDL_SCANCODE_W)
                {
                    y_pos -= 1.5f;
                    y_pos *= x_factor;

                }

                else if (event.key.scancode == SDL_SCANCODE_S)
                {
                    y_pos += 1.5f;
                    y_pos *= y_factor;

                }

                else if (event.key.scancode == SDL_SCANCODE_A)
                {
                    x_pos -= 1.5f;
                    x_pos *= x_factor;

                }

                else if (event.key.scancode == SDL_SCANCODE_D)
                {
                    x_pos += 1.5f;
                    x_pos *= x_factor;

                }

                /* the pressed key was Escape? */
                if (event.key.key == SDLK_ESCAPE)
                {
                    done = true;
                }
            }
        }

        // Clear screen (black)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw a red rectangle (use SDL_FRect for SDL3 renderer API)
        SDL_FRect rectf = {x_pos, y_pos, 50.0f, 50.0f};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rectf);

        // Present the backbuffer
        SDL_RenderPresent(renderer);
    }

    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window)
        SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}