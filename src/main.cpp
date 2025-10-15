#include <iostream>
#include <map>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <SDL3_image/SDL_image.h>

#define WINDOW_WIDTH 1366
#define WINDOW_HEIGHT 768

std::map<SDL_Keycode, bool> keyMap;

float speed = 0.5f;

// Rectangle properties
float x_pos = 0.0f;
float y_pos = 0.0f;

float width_rect = 50.0f;
float height_rect = 50.0f;

void wasd_input(std::map<SDL_Keycode, bool> &keyMap, float &speed, int &width, int &height)
{

    if (keyMap[SDLK_S])
    {
        y_pos += speed;

        if (y_pos + height_rect >= height)
        {
            y_pos -= speed;
        }
    }

    if (keyMap[SDLK_W])
    {
        y_pos -= speed;

        if (y_pos <= 0)
        {
            y_pos += speed;
        }
    }

    if (keyMap[SDLK_A])
    {
        x_pos -= speed;

        if (x_pos <= 0)
        {
            x_pos += speed;
        }
    }

    if (keyMap[SDLK_D])
    {
        x_pos += speed;

        if (x_pos + width_rect >= width)
        {
            x_pos -= speed;
        }
    }
}

int main(int argc, char *argv[])
{

    SDL_Window *window = SDL_CreateWindow(
        "Ship Builder",
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_FULLSCREEN // plain window for the SDL renderer
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

    int width = 0, height = 0;
    int num_displays;
    SDL_DisplayID *displays = SDL_GetDisplays(&num_displays);

    const SDL_DisplayMode *dm = SDL_GetCurrentDisplayMode(*displays);

    if (dm == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not get the current display mode: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_free(displays);

    width = dm->w;
    height = dm->h;

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
                keyMap[event.key.key] = true;
            }

            if (event.type == SDL_EVENT_KEY_UP)
            {
                keyMap[event.key.key] = false;
            }
        }

        wasd_input(keyMap, speed, width, height);

        //Collision detection

        if (keyMap[SDLK_ESCAPE])
        {
            done = true;
        }

        // Clear screen (black)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw a red rectangle (use SDL_FRect for SDL3 renderer API)
        SDL_FRect rectf = {x_pos, y_pos, width_rect, height_rect};

        SDL_FRect rectf2 = {500.0f, 500.0f, 100.0f, 100.0f};

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rectf);

        SDL_SetRenderDrawColor(renderer, 150, 255, 0, 255);
        SDL_RenderFillRect(renderer, &rectf2);

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