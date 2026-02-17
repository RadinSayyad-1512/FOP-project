#include "../include/Block.h"

Block::Block(BlockType t, std::string txt, float startX, float startY) {
    type = t;
    text = txt;
    x = startX;
    y = startY;
    w = 170.0f;
    h = 45.0f;
}

void Block::render(SDL_Renderer* renderer) {
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
        case BlockType::Sensing:
            SDL_SetRenderDrawColor(renderer, 76, 177, 214, 255);
            break;
        case BlockType::Operators:
            SDL_SetRenderDrawColor(renderer, 64, 191, 74, 255);
            break;
        case BlockType::Variables:
            SDL_SetRenderDrawColor(renderer, 255, 140, 26, 255);
            break;
        default:
            SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
            break;
    }

    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 60);
    SDL_RenderDrawRect(renderer, &rect);
}