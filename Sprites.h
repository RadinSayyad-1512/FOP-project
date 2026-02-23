#ifndef SPRITES_H
#define SPRITES_H

#include <SDL2/SDL.h>
#include <vector>
#include <string>

enum Direction { UP, DOWN, LEFT, RIGHT };

struct sprite {
    SDL_Rect rect;
    int dir;
    int alpha;
    float scale;
    SDL_Texture* activeBubble;

    bool isPenDown;
    SDL_Color penColor;
    int penSize;
    std::vector<SDL_Texture*> costumes;
    int activeCostume = 0;
};

void initSprite(sprite &s, float x, float y);
void renderSprite(sprite &s, SDL_Renderer* ren);

#endif