#include "../include/Block.h"

Block::Block(BlockType t, std::string txt, float startX, float startY) {
    type = t;
    text = txt;
    x = startX;
    y = startY;
    w = 160.0f;
    h = 45.0f;

    if (type == BlockType::Events)
        color = {255,191,0,255};
    else if (type == BlockType::Motion)
        color = {76,151,255,255};
    else if (type == BlockType::Looks)
        color = {153,102,255,255};
    else if (type == BlockType::Control)
        color = {255,171,25,255};
    else
        color = {150,150,150,255};
}

void Block::render(SDL_Renderer* renderer) {

    SDL_Rect rect = {(int)x, (int)y, (int)w, (int)h};

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);

    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);

}