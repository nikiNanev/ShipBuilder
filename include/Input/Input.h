#include <SDL3/SDL.h>
#include <map>

#include "Ship/Ship.h"
#include "Ship/Station.h"

class Input
{

public:
    Input();
    ~Input();

    // Keyboard
    void wasd_keyboard(std::map<SDL_Keycode, bool> &keyMap, struct ship *ship, struct station *station, int &width, int &height);
};