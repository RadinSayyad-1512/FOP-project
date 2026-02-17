#include "../include/Block.h"

Block::Block(BlockType t, std::string txt, float startX, float startY) {
    type = t;
    text = txt;
    x = startX;
    y = startY;
    w = 170.0f;
    h = 45.0f;
}

void Block::render(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Rect rect = { (int)x, (int)y, (int)w, (int)h };

    switch (type) {
        case BlockType::Motion:
            SDL_SetRenderDrawColor(renderer, 76, 151, 255, 255);
            break;
        case BlockType::Looks:
            SDL_SetRenderDrawColor(renderer, 153, 102, 255, 255);
            break;
        case BlockType::Events:
            SDL_SetRenderDrawColor(renderer, 255, 191, 0, 255);
            break;
        case BlockType::Control:
            SDL_SetRenderDrawColor(renderer, 255, 171, 25, 255);
            break;
        default:
            SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
            break;
    }

    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 60);
    SDL_RenderDrawRect(renderer, &rect);

    if (font != nullptr && !text.empty()) {
        SDL_Color textColor = { 255, 255, 255, 255 };

        SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);

        if (textSurface) {
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

            int textW = textSurface->w;
            int textH = textSurface->h;
            SDL_Rect textRect = { (int)(x + 10), (int)(y + (h - textH) / 2), textW, textH };

            SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
        }
    }
}