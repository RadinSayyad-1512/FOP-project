#include "Block.h"

Block createBlock(int x, int y, int w, int h, ActionType action, SDL_Texture* tex) {
    Block b;
    b.rect = {x, y, w, h};
    b.action = action;
    b.texture = tex;
    return b;
}