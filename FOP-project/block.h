#ifndef BLOCK_H
#define BLOCK_H

#include <SDL2/SDL.h>

// All actions current and future
enum ActionType {
    MOVE, SAY, THINK,
    SET_UP, SET_DOWN, SET_LEFT, SET_RIGHT,
    HIDE, SHOW,
    // --- PEN EXTENSION ---
    PEN_DOWN, PEN_UP, ERASE_ALL, STAMP,
    SET_PEN_RED, SET_PEN_BLUE, PEN_SIZE_INC, PEN_SIZE_DEC,
    // --- MOTION ADDITIONS ---
    GOTO_RANDOM, GOTO_MOUSE
};

struct Block {
    SDL_Rect rect;
    ActionType action;
    SDL_Texture* texture;
};

// Helper to bundle data into a Block
Block createBlock(int x, int y, int w, int h, ActionType action, SDL_Texture* tex);

#endif