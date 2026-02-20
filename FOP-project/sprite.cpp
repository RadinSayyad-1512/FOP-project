#include "Sprites.h"

void initSprite(sprite &s, float x, float y) {
    s.rect = {(int)x, (int)y, 80, 80};
    s.activeBubble = nullptr;
    s.dir = RIGHT;
    s.alpha = 255;
}

void renderSprite(sprite &s, SDL_Renderer* ren) {
    if (s.alpha <= 0) return;

    SDL_SetRenderDrawColor(ren, 255, 50, 50, s.alpha);
    SDL_RenderFillRect(ren, &s.rect);
    SDL_SetRenderDrawColor(ren, 0, 0, 0, s.alpha);
    SDL_RenderDrawRect(ren, &s.rect);

    if (s.activeBubble) {
        SDL_Rect bubbleRect = {s.rect.x - 20, s.rect.y - 90, 120, 80};
        SDL_SetTextureAlphaMod(s.activeBubble, s.alpha);
        SDL_RenderCopy(ren, s.activeBubble, NULL, &bubbleRect);
    }
}