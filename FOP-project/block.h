#ifndef BLOCK_H
#define BLOCK_H

#include <SDL2/SDL.h>

enum ActionType {
    MOVE, SAY, THINK,
    SET_UP, SET_DOWN, SET_LEFT, SET_RIGHT,
    HIDE, SHOW
};

struct Block {
    SDL_Rect rect;
    ActionType action;
    SDL_Texture* texture;
};

Block createBlock(int x, int y, int w, int h, ActionType action, SDL_Texture* tex);

#endif