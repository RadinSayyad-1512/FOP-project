#include "Sprites.h"

void initSprite(sprite &s, float x, float y) {
    s.rect = {(int)x, (int)y, 80, 80};
    s.activeBubble = nullptr;
    s.dir = RIGHT;
    s.alpha = 255;
    s.scale = 1.0f; // <--- ADD THIS LINE

    s.isPenDown = false;
    s.penColor = {0, 0, 255, 255};
    s.penSize = 4;
}

void renderSprite(sprite &s, SDL_Renderer* ren) {
    // Calculate dynamic width and height based on scale
    int w = (int)(80 * s.scale);
    int h = (int)(80 * s.scale);

    // This is the rectangle we actually draw
    SDL_Rect drawRect = { s.rect.x, s.rect.y, w, h };

    // 1. Draw the Sprite Body
    if (s.alpha > 0) {
        // Apply transparency to the fill
        SDL_SetRenderDrawColor(ren, 255, 50, 50, s.alpha);
        SDL_RenderFillRect(ren, &drawRect); // Use drawRect, not s.rect

        SDL_SetRenderDrawColor(ren, 0, 0, 0, s.alpha);
        SDL_RenderDrawRect(ren, &drawRect); // Use drawRect, not s.rect
    }

    // 2. Draw the Bubble
    if (s.activeBubble) {
        // We position the bubble relative to the NEW size
        // This keeps it floating centered above the sprite
        SDL_Rect bubbleRect = { drawRect.x + (w / 2) - 60, drawRect.y - 90, 120, 80 };

        SDL_SetTextureAlphaMod(s.activeBubble, 255);
        SDL_RenderCopy(ren, s.activeBubble, NULL, &bubbleRect);
    }
}