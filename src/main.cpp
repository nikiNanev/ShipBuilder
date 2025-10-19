#include <iostream>
#include <string>
#include <memory>
#include <map>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#include "Input/Input.h"

#define WINDOW_WIDTH 1366
#define WINDOW_HEIGHT 768

std::map<SDL_Keycode, bool> keyMap;

void debug_coords(struct ship &shipA, struct station &stationA, SDL_Renderer *renderer)
{
    std::string x_position = "ship x: ";
    std::string current_x_position = std::to_string(shipA.x_pos);
    std::string xPosition_full = x_position + current_x_position;

    std::string y_position = "ship y: ";
    std::string current_y_position = std::to_string(shipA.y_pos);
    std::string yPosition_full = y_position + current_y_position;

    std::string station_xPos = "station x: ";
    std::string station_curr_xPos = std::to_string(stationA.x_pos);
    std::string station_xPos_full = station_xPos + station_curr_xPos;

    std::string station_yPos = "station y: ";
    std::string station_curr_yPos = std::to_string(stationA.y_pos);
    std::string station_yPos_full = station_yPos + station_curr_yPos;

    // Debug Info ( coords )
    SDL_RenderDebugText(renderer, 10, 10, xPosition_full.c_str());
    SDL_RenderDebugText(renderer, 10, 30, yPosition_full.c_str());

    SDL_RenderDebugText(renderer, 250, 10, station_xPos_full.c_str());
    SDL_RenderDebugText(renderer, 250, 30, station_yPos_full.c_str());
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

    SDL_free(displays);

    if (dm == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not get the current display mode: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

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

    auto input = std::make_unique<Input>();

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

        input->wasd_keyboard(keyMap, &shipA, &stationA, width, height);

        if (keyMap[SDLK_ESCAPE])
        {
            done = true;
        }

        // Clear screen (black)
        SDL_SetRenderDrawColor(renderer, 0, 10, 0, 255);
        SDL_RenderClear(renderer);

        // Draw a red rectangle (use SDL_FRect for SDL3 renderer API)
        SDL_FRect shipA_rect = {shipA.x_pos, shipA.y_pos, shipA.width, shipA.height};

        SDL_FRect stationA_rect = {stationA.x_pos, stationA.y_pos, stationA.width, stationA.height};

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &shipA_rect);

        SDL_SetRenderDrawColor(renderer, 150, 255, 0, 255);
        SDL_RenderFillRect(renderer, &stationA_rect);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        debug_coords(shipA, stationA, renderer);

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