#ifndef SPRITES_H
#define SPRITES_H

#include <SDL2/SDL.h>

enum Direction { UP, DOWN, LEFT, RIGHT };

struct sprite {
    SDL_Rect rect;
    int dir;
    int alpha;
    SDL_Texture* texture;
    SDL_Texture* activeBubble;
    bool isPenDown;
    SDL_Color penColor;
    int penSize;
    float scale = 1.0f;
};

void initSprite(sprite &s, float x, float y);
void renderSprite(sprite &s, SDL_Renderer* ren);

#endif