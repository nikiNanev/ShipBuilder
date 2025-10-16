#include <iostream>
#include <string>
#include <map>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <SDL3_image/SDL_image.h>

#define WINDOW_WIDTH 1366
#define WINDOW_HEIGHT 768

std::map<SDL_Keycode, bool> keyMap;

struct ship
{
    float x_pos;
    float y_pos;

    float width, height;

    float speed;
};

struct station
{
    float x_pos, y_pos;
    float width, height;
};

void wasd_input(std::map<SDL_Keycode, bool> &keyMap, struct ship *ship, struct station *station, int &width, int &height)
{

    if (keyMap[SDLK_S])
    {
        ship->y_pos += ship->speed;

        if (ship->y_pos + ship->height >= height)
        {
            ship->y_pos -= ship->speed;
        }

        // Collision detection ( is upper line of the station intersects with bottom line of the ship? )

        if (station->y_pos <= ship->y_pos + ship->height)
        {
            if (!(ship->x_pos + ship->width <= (station->x_pos) || ship->x_pos >= (station->x_pos + station->width)))
            {
                ship->y_pos -= ship->speed;
            }
        }
    }

    if (keyMap[SDLK_W])
    {
        ship->y_pos -= ship->speed;

        if (ship->y_pos <= 0)
        {
            ship->y_pos += ship->speed;
        }

        // Collision detection ( is bottom line of the station intersects with the upper line of the ship? )

        if (station->y_pos + station->height >= ship->y_pos)
        {

            if (!(ship->x_pos + ship->width <= (station->x_pos) || ship->x_pos >= (station->x_pos + station->width)))
            {
                ship->y_pos += ship->speed;

                std::cout << "ship yPos + height: " << ship->y_pos + ship->height << std::endl;

                std::cout << "station yPos + height: " << station->y_pos + station->height << std::endl;
            }
        }
    }

    if (keyMap[SDLK_A])
    {
        ship->x_pos -= ship->speed;

        if (ship->x_pos <= 0)
        {
            ship->x_pos += ship->speed;
        }

        // Collision detection ( is right side of the station intersects with the left side of the ship? )

        if (station->x_pos <= ship->x_pos)
        {

            if ((station->x_pos + station->width >= ship->x_pos))
            {
                std::cout << "station xPos + width: " << station->x_pos + station->width << std::endl;
                std::cout << "ship xPos: " << ship->x_pos << std::endl;
            }

        }

    }

    if (keyMap[SDLK_D])
    {
        ship->x_pos += ship->speed;

        if (ship->x_pos + ship->width >= width)
        {
            ship->x_pos -= ship->speed;
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

    struct ship shipA;

    shipA.x_pos = 0;
    shipA.y_pos = 0;

    shipA.height = 50.0f;
    shipA.width = 50.0f;

    shipA.speed = 0.35f;

    struct station stationA;

    stationA.x_pos = 500.0f;
    stationA.y_pos = 500.0f;

    stationA.width = 100.0f;
    stationA.height = 100.0f;

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

        wasd_input(keyMap, &shipA, &stationA, width, height);

        if (keyMap[SDLK_ESCAPE])
        {
            done = true;
        }

        // Clear screen (black)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw a red rectangle (use SDL_FRect for SDL3 renderer API)
        SDL_FRect shipA_rect = {shipA.x_pos, shipA.y_pos, shipA.width, shipA.height};

        SDL_FRect stationA_rect = {stationA.x_pos, stationA.y_pos, stationA.width, stationA.height};

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &shipA_rect);

        SDL_SetRenderDrawColor(renderer, 150, 255, 0, 255);
        SDL_RenderFillRect(renderer, &stationA_rect);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        std::string x_position = "x: ";
        std::string current_x_position = std::to_string(shipA.x_pos);
        std::string xPosition_full = x_position + current_x_position;

        std::string y_position = "y: ";
        std::string current_y_position = std::to_string(shipA.y_pos);
        std::string yPosition_full = y_position + current_y_position;



        SDL_RenderDebugText(renderer, 10, 10, xPosition_full.c_str());
        SDL_RenderDebugText(renderer, 10, 30, yPosition_full.c_str());

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