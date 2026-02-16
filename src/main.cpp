#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <vector>
#include "../include/Block.h"

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Sharif Scratch - Phase 1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    std::vector<Block> myBlocks;
    myBlocks.push_back(Block(BlockType::Events, "When Flag Clicked", 40, 60));
    myBlocks.push_back(Block(BlockType::Motion, "Move 10 Steps", 40, 120));
    myBlocks.push_back(Block(BlockType::Looks, "Say Hello", 40, 180));
    myBlocks.push_back(Block(BlockType::Control, "Wait 1 Sec", 40, 240));

    bool isRunning = true;
    SDL_Event event;

    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) isRunning = false;
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_Rect sidebar = {0, 0, 250, 768};
        SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
        SDL_RenderFillRect(renderer, &sidebar);

        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderDrawLine(renderer, 250, 0, 250, 768);

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