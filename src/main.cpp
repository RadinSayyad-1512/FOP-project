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

    std::vector<Block*> workspaceBlocks;
    std::vector<Block*> menuTemplates;
    menuTemplates.push_back(new Block(BlockType::Events, "When Flag Clicked", 40, 60));
    menuTemplates.push_back(new Block(BlockType::Motion, "Move 10 Steps", 40, 120));
    menuTemplates.push_back(new Block(BlockType::Looks, "Say Hello", 40, 180));
    menuTemplates.push_back(new Block(BlockType::Control, "Wait 1 Sec", 40, 240));

    Block* draggedBlock = nullptr;
    int offsetX = 0, offsetY = 0;

    bool isRunning = true;
    SDL_Event event;

    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                isRunning = false;

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;

                for (auto* templateBlock : menuTemplates) {
                    if (mouseX >= templateBlock->x && mouseX <= templateBlock->x + templateBlock->w &&
                        mouseY >= templateBlock->y && mouseY <= templateBlock->y + templateBlock->h) {
                        Block* newBlock = new Block(templateBlock->type, templateBlock->text, templateBlock->x, templateBlock->y);
                        workspaceBlocks.push_back(newBlock);
                        draggedBlock = newBlock;

                        offsetX = mouseX - (int)draggedBlock->x;
                        offsetY = mouseY - (int)draggedBlock->y;
                        break;
                    }
                }
                if (draggedBlock == nullptr) {
                    for (int i = workspaceBlocks.size() - 1; i >= 0; i--) {
                        Block* b = workspaceBlocks[i];
                        if (mouseX >= b->x && mouseX <= b->x + b->w &&
                            mouseY >= b->y && mouseY <= b->y + b->h) {
                            draggedBlock = b;

                            if (draggedBlock->prev) {
                                draggedBlock->prev->next = nullptr;
                                draggedBlock->prev = nullptr;
                            }

                            offsetX = mouseX - (int)draggedBlock->x;
                            offsetY = mouseY - (int)draggedBlock->y;
                            break;
                        }
                    }
                }
            }


            if (event.type == SDL_MOUSEBUTTONUP) {
               if (draggedBlock != nullptr) {
                   if (event.button.x < 250) {
                       auto it = workspaceBlocks.begin();
                       while (it != workspaceBlocks.end()) {

                           if (*it == draggedBlock) {
                               delete *it;
                               it = workspaceBlocks.erase(it);
                           }
                           else {
                               it++;
                           }
                       }
                   }
                   else{
                       bool snapped = false;
                       for (Block* otherBlock : workspaceBlocks) {
                           if (draggedBlock == otherBlock)
                               continue;

                           float snappToBottomX = abs(draggedBlock->x - otherBlock->x);
                           float snappToBottomY = abs(draggedBlock->y - otherBlock->y - otherBlock->h);

                           if (snappToBottomX < 35 && snappToBottomY < 35 && otherBlock->next == nullptr) {
                               otherBlock->next = draggedBlock;
                               draggedBlock->prev = otherBlock;
                               draggedBlock->updatePosition(otherBlock->x, otherBlock->y + otherBlock->h);
                               snapped = true;
                               break;
                           }

                           float snappToTopX = abs(draggedBlock->x - otherBlock->x);
                           float snappToTopY = abs(draggedBlock->y + draggedBlock->h - otherBlock->y);

                           if (snappToTopX < 35 && snappToTopY < 35 && otherBlock->prev == nullptr) {
                               draggedBlock->next = otherBlock;
                               otherBlock->prev = draggedBlock;
                               draggedBlock->updatePosition(otherBlock->x, otherBlock->y - draggedBlock->h);
                               snapped = true;
                               break;

                           }
                       }
                   }
               }
                draggedBlock = nullptr;
            }
            if (event.type == SDL_MOUSEMOTION && draggedBlock != nullptr) {
                draggedBlock->updatePosition(event.motion.x - offsetX, event.motion.y - offsetY);
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_Rect sidebar = {0, 0, 250, 768};
        SDL_SetRenderDrawColor(renderer, 242, 242, 242, 255);
        SDL_RenderFillRect(renderer, &sidebar);

        SDL_SetRenderDrawColor(renderer, 210, 210, 210, 255);
        SDL_RenderDrawLine(renderer, 250, 0, 250, 768);

        for (auto* block : menuTemplates) {
            block->render(renderer, myFont);
        }

        for (auto* block : workspaceBlocks) {
            block-> render(renderer, myFont);
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