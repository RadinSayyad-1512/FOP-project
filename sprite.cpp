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

    int w = (int)(80 * s.scale);
    int h = (int)(80 * s.scale);

    SDL_Rect drawRect = { s.rect.x, s.rect.y, w, h };
    if (s.alpha > 0 && !s.costumes.empty()) {
        SDL_Texture* currentTex = s.costumes[s.activeCostume];


        SDL_SetTextureAlphaMod(currentTex, s.alpha);


        SDL_RendererFlip flip = (s.dir == LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

        SDL_RenderCopyEx(ren, currentTex, NULL, &drawRect, 0, NULL, flip);
    }

    if (s.activeBubble) {

        SDL_Rect bubbleRect = { drawRect.x + (w / 2) - 60, drawRect.y - 90, 120, 80 };

        SDL_SetTextureAlphaMod(s.activeBubble, 255);
        SDL_RenderCopy(ren, s.activeBubble, NULL, &bubbleRect);
    }
}