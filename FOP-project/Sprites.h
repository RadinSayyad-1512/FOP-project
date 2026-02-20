#ifndef SPRITES_H
#define SPRITES_H

#include <SDL2/SDL.h>

enum Direction { UP, DOWN, LEFT, RIGHT };

struct sprite {
    SDL_Rect rect;
    SDL_Texture* activeBubble;
    Direction dir;
    int alpha;
};

void initSprite(sprite &s, float x, float y);
void renderSprite(sprite &s, SDL_Renderer* ren);

#endif