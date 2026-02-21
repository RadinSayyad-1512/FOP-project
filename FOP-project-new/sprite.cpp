#include "Sprites.h"

void initSprite(sprite &s, float x, float y) {
    s.rect = {(int)x, (int)y, 80, 80};
    s.activeBubble = nullptr;
    s.dir = RIGHT;
    s.alpha = 255;

    s.isPenDown = false;
    s.penColor = {0, 0, 255, 255}; // Blue
    s.penSize = 4;
}

void renderSprite(sprite &s, SDL_Renderer* ren) {
    // 1. Draw the Sprite Body ONLY if visible
    if (s.alpha > 0) {
        SDL_SetRenderDrawColor(ren, 255, 50, 50, s.alpha);
        SDL_RenderFillRect(ren, &s.rect);

        SDL_SetRenderDrawColor(ren, 0, 0, 0, s.alpha);
        SDL_RenderDrawRect(ren, &s.rect);
    }

    // 2. Draw the Bubble (Always 255 Alpha)
    if (s.activeBubble) {
        SDL_Rect bubbleRect = {s.rect.x - 20, s.rect.y - 90, 120, 80};
        SDL_SetTextureAlphaMod(s.activeBubble, 255);
        SDL_RenderCopy(ren, s.activeBubble, NULL, &bubbleRect);
    }
}