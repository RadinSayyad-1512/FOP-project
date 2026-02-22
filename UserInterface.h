//
// Created by Amirmohammad Mirzaei on 2/22/2026.
//

#ifndef SDL2_USERINTERFACE_H
#define SDL2_USERINTERFACE_H

#include <SDL2/SDL.h>
#include "Sprites.h"

// A function to draw an information box
void drawInfoBox(SDL_Renderer* ren, const sprite& s);

//A function to draw borders for the entire page
void drawUIFrames(SDL_Renderer* ren);


#endif //SDL2_USERINTERFACE_H
