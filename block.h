#ifndef BLOCK_H
#define BLOCK_H

#include <SDL2/SDL.h>

enum ActionType {
    MOVE, SAY, THINK, HIDE, SHOW,
    SET_UP, SET_DOWN, SET_LEFT, SET_RIGHT,
    GOTO_RANDOM, GOTO_MOUSE,
    PEN_DOWN, PEN_UP, ERASE_ALL, STAMP,
    SET_PEN_RED, SET_PEN_BLUE, PEN_SIZE_INC, PEN_SIZE_DEC,
    REPEAT_BEGIN, REPEAT_END , SIZE_INC, SIZE_DEC, SIZE_RESET,
    WAIT_SEC,
    TIMER_RESET, TIMER_SAY ,NEXT_BG , NEXT_COSTUME , PLAY_POP
};


struct Block {
    SDL_Rect rect;
    ActionType action;
    SDL_Texture* texture;
};

// Helper to bundle data into a Block
Block createBlock(int x, int y, int w, int h, ActionType action, SDL_Texture* tex);

#endif