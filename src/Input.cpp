#include "Input/Input.h"

Input::Input()
{
}

Input::~Input()
{
}

void Input::wasd_keyboard(std::map<SDL_Keycode, bool> &keyMap, struct ship *ship, struct station *station, int &width, int &height)
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
        if (!(ship->x_pos + ship->width <= (station->x_pos) || ship->x_pos >= (station->x_pos + station->width)))
        {

            if (station->y_pos + station->height >= ship->y_pos)
            {
                ship->y_pos += ship->speed;
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

        if (station->x_pos >= ship->x_pos + ship->width)
        {
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