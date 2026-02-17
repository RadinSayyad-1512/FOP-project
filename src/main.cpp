#define SDL_MAIN_HANDLED
#include "../include/Block.h"

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("FOP Project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                        1024, 768, SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    TTF_Font* myFont = TTF_OpenFont("arial.ttf", 16);
    if (!myFont) {
        std::cerr << "Failed to load font! Error: " << TTF_GetError() << std::endl;
    }

    std::vector<Block> myBlocks;
    myBlocks.push_back(Block(BlockType::Events, "When Flag Clicked", 40, 60));
    myBlocks.push_back(Block(BlockType::Motion, "Move 10 Steps", 40, 120));
    myBlocks.push_back(Block(BlockType::Looks, "Say Hello", 40, 180));
    myBlocks.push_back(Block(BlockType::Control, "Wait 1 Sec", 40, 240));

    Block* draggedBlock = nullptr;
    int offsetX = 0, offsetY = 0;

    bool isRunning = true;
    SDL_Event event;

    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                isRunning = false;

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;

                    for (int i = myBlocks.size() - 1 ; i >= 0; i--) {
                        if (mouseX >= myBlocks[i].x && mouseX <= (myBlocks[i].x + myBlocks[i].w) &&
                            mouseY >= myBlocks[i].y && mouseY <= (myBlocks[i].y + myBlocks[i].h)) {

                            draggedBlock = &myBlocks[i];
                            offsetX = mouseX - (int)draggedBlock->x;
                            offsetY = mouseY - (int)draggedBlock->y;
                            break;
                        }
                    }
                }
            }

            if (event.type == SDL_MOUSEBUTTONUP)
                draggedBlock = nullptr;

            if (event.type == SDL_MOUSEMOTION && draggedBlock != nullptr) {
                draggedBlock->x = (float)(event.motion.x - offsetX);
                draggedBlock->y = (float)(event.motion.y - offsetY);
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_Rect sidebar = {0, 0, 250, 768};
        SDL_SetRenderDrawColor(renderer, 242, 242, 242, 255);
        SDL_RenderFillRect(renderer, &sidebar);

        SDL_SetRenderDrawColor(renderer, 210, 210, 210, 255);
        SDL_RenderDrawLine(renderer, 250, 0, 250, 768);

        for (auto& block : myBlocks) {
            block.render(renderer, myFont);
        }

        SDL_RenderPresent(renderer);
    }

    if (myFont)
        TTF_CloseFont(myFont);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}