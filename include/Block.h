#ifndef FOP_PROJECT_BLOCK_H
#define FOP_PROJECT_BLOCK_H

#include <SDL2/SDL.h>
#include <string>

enum class BlockType {
    Motion, Looks, Sound, Events, Control, Sensing, Operators, Variables
};

class Block {
public:
    float x, y, w, h;
    BlockType type;
    SDL_Color color;
    std::string text;

    Block(BlockType t, std::string txt, float startX, float startY);

    void render(SDL_Renderer* renderer);
};

#endif