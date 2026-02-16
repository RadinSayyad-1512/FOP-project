#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include "../include/Block.h"

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL Initialization Failed: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("FOP Project", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                            1024, 768, SDL_WINDOW_SHOWN);
    if (!window)
        return -1;

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    std::vector<Block> myBlocks;

    myBlocks.push_back(Block(BlockType::Events, "When Flag Clicked", 50, 50));
    myBlocks.push_back(Block(BlockType::Motion, "Move 10 Steps", 50, 100));
    myBlocks.push_back(Block(BlockType::Looks, "Say Hello", 50, 150));
    myBlocks.push_back(Block(BlockType::Control, "Forever", 50, 200));

    bool isRunning = true;
    SDL_Event event;

    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_Rect sidebar = {0, 0, 250, 768};
        SDL_SetRenderDrawColor(renderer, 230, 230, 230, 255);
        SDL_RenderFillRect(renderer, &sidebar);

        for (auto& block : myBlocks) {
            block.render(renderer);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
