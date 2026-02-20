#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include "Block.h"
#include "Sprites.h"

const int SCREEN_W = 1200;
const int SCREEN_H = 800;
const int DIV_X = 400;
const int DIV_Y = 400;

struct PaletteItem { ActionType type; SDL_Texture* tex; };
std::vector<PaletteItem> allPaletteItems;
int currentPage = 0;
const int BLOCKS_PER_PAGE = 4;

// Helper to draw triangles for pagination
void drawTriangle(SDL_Renderer* ren, int x, int y, bool left) {
    SDL_Point points[4];
    if (left) {
        points[0] = {x, y}; points[1] = {x + 20, y - 10};
        points[2] = {x + 20, y + 10}; points[3] = {x, y};
    } else {
        points[0] = {x, y}; points[1] = {x - 20, y - 10};
        points[2] = {x - 20, y + 10}; points[3] = {x, y};
    }
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    SDL_RenderDrawLines(ren, points, 4);
}

// THE FIX: Centralized Render Function
void renderAll(SDL_Renderer* ren, sprite& player, const std::vector<Block>& workspace, Block* dragging, bool isRunning, SDL_Texture* tNew, SDL_Rect& resetBtn, SDL_Rect& startBtn) {
    SDL_SetRenderDrawColor(ren, 20, 20, 20, 255);
    SDL_RenderClear(ren);

    // 1. Draw UI Backgrounds
    SDL_SetRenderDrawColor(ren, 60, 60, 60, 255);
    SDL_Rect sideTop = {0,0, DIV_X, DIV_Y}; SDL_RenderFillRect(ren, &sideTop);
    SDL_SetRenderDrawColor(ren, 40, 40, 40, 255);
    SDL_Rect sideBot = {0, DIV_Y, DIV_X, DIV_Y}; SDL_RenderFillRect(ren, &sideBot);

    // 2. Draw Dividers
    SDL_SetRenderDrawColor(ren, 100, 100, 100, 255);
    SDL_RenderDrawLine(ren, DIV_X, 0, DIV_X, SCREEN_H);
    SDL_RenderDrawLine(ren, 0, DIV_Y, DIV_X, DIV_Y);

    // 3. Draw Buttons
    SDL_RenderCopy(ren, tNew, NULL, &resetBtn);
    if (isRunning) SDL_SetRenderDrawColor(ren, 255, 0, 0, 255); // Red when busy
    else SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);           // Green when ready
    SDL_RenderFillRect(ren, &startBtn);

    drawTriangle(ren, 370, 365, false); // Next
    drawTriangle(ren, 30, 365, true);   // Prev

    // 4. Draw Current Palette Page
    for (int i=0; i < BLOCKS_PER_PAGE; i++) {
        int idx = currentPage * BLOCKS_PER_PAGE + i;
        if (idx >= allPaletteItems.size()) break;
        SDL_Rect r = {100, 60 + (i*70), 200, 50};
        SDL_RenderCopy(ren, allPaletteItems[idx].tex, NULL, &r);
    }

    // 5. Draw Workspace Blocks
    for (auto& b : workspace) SDL_RenderCopy(ren, b.texture, NULL, &b.rect);

    // 6. Draw Sprite
    renderSprite(player, ren);

    // 7. Draw Dragging Block
    if (dragging) {
        SDL_RenderCopy(ren, dragging->texture, NULL, &dragging->rect);
    }

    SDL_RenderPresent(ren);
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    SDL_Window* win = SDL_CreateWindow("Mini Scratch v2", 100, 100, SCREEN_W, SCREEN_H, 0);
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);

    // Load Textures
    SDL_Texture* tMove = IMG_LoadTexture(ren, "move_btn.png");
    SDL_Texture* tSay = IMG_LoadTexture(ren, "say_btn.png");
    SDL_Texture* tThink = IMG_LoadTexture(ren, "think_btn.png");
    SDL_Texture* tUp = IMG_LoadTexture(ren, "dir_up_btn.png");
    SDL_Texture* tDown = IMG_LoadTexture(ren, "dir_down_btn.png");
    SDL_Texture* tL = IMG_LoadTexture(ren, "dir_left_btn.png");
    SDL_Texture* tR = IMG_LoadTexture(ren, "dir_right_btn.png");
    SDL_Texture* tHide = IMG_LoadTexture(ren, "hide_btn.png");
    SDL_Texture* tShow = IMG_LoadTexture(ren, "show_btn.png");
    SDL_Texture* tHiBub = IMG_LoadTexture(ren, "hi_bubble.png");
    SDL_Texture* tWhyBub = IMG_LoadTexture(ren, "why_bubble.png");
    SDL_Texture* tNew = IMG_LoadTexture(ren, "new_project.png");

    allPaletteItems = { {MOVE, tMove}, {SAY, tSay}, {THINK, tThink}, {HIDE, tHide},
                        {SHOW, tShow}, {SET_UP, tUp}, {SET_DOWN, tDown}, {SET_LEFT, tL}, {SET_RIGHT, tR} };

    sprite player;
    initSprite(player, 700, 400);
    std::vector<Block> workspace;
    Block* dragging = nullptr;

    SDL_Rect nextBtn = {350, 350, 30, 30};
    SDL_Rect prevBtn = {20, 350, 30, 30};
    SDL_Rect startBtn = {550, 20, 50, 50};
    SDL_Rect resetBtn = {20, 20, 100, 30};

    bool isRunning = false;
    bool quit = false;
    SDL_Event e;

    while (!quit) {
        int mx, my;
        SDL_GetMouseState(&mx, &my);

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
            if (e.type == SDL_MOUSEBUTTONDOWN && !isRunning) {
                SDL_Point p = {mx, my};
                if (SDL_PointInRect(&p, &nextBtn) && (currentPage+1)*BLOCKS_PER_PAGE < allPaletteItems.size()) currentPage++;
                if (SDL_PointInRect(&p, &prevBtn) && currentPage > 0) currentPage--;
                if (SDL_PointInRect(&p, &resetBtn)) { workspace.clear(); initSprite(player, 700, 400); }
                if (SDL_PointInRect(&p, &startBtn)) isRunning = true;
                for (int i=0; i < BLOCKS_PER_PAGE; i++) {
                    int idx = currentPage * BLOCKS_PER_PAGE + i;
                    if (idx >= allPaletteItems.size()) break;
                    SDL_Rect r = {100, 60 + (i*70), 200, 50};
                    if (SDL_PointInRect(&p, &r))
                        dragging = new Block(createBlock(mx-100, my-25, 200, 50, allPaletteItems[idx].type, allPaletteItems[idx].tex));
                }
            }
            if (e.type == SDL_MOUSEBUTTONUP && dragging) {
                if (mx < DIV_X && my > DIV_Y) {
                    int nextY = workspace.empty() ? DIV_Y + 20 : workspace.back().rect.y + 55;
                    dragging->rect = {100, nextY, 200, 50};
                    workspace.push_back(*dragging);
                }
                delete dragging; dragging = nullptr;
            }
        }

        if (isRunning) {
            for (auto& b : workspace) {
                if (b.action == MOVE) {
                    for(int step=0; step<2; step++) { // 2 distinct steps of 10 pixels
                        if (player.dir == RIGHT) player.rect.x += 10;
                        else if (player.dir == LEFT)  player.rect.x -= 10;
                        else if (player.dir == UP)    player.rect.y -= 10;
                        else if (player.dir == DOWN)  player.rect.y += 10;
                        renderAll(ren, player, workspace, nullptr, true, tNew, resetBtn, startBtn);
                        SDL_Delay(200);
                    }
                } else {
                    if (b.action == SAY) player.activeBubble = tHiBub;
                    else if (b.action == THINK) player.activeBubble = tWhyBub;
                    else if (b.action == HIDE) player.alpha = 0;
                    else if (b.action == SHOW) player.alpha = 255;
                    else if (b.action == SET_UP) player.dir = UP;
                    else if (b.action == SET_DOWN) player.dir = DOWN;
                    else if (b.action == SET_LEFT) player.dir = LEFT;
                    else if (b.action == SET_RIGHT) player.dir = RIGHT;

                    renderAll(ren, player, workspace, nullptr, true, tNew, resetBtn, startBtn);
                    SDL_Delay(400); // Wait longer so we can actually see the bubble/change
                }
            }
            isRunning = false;
        }

        if (dragging) { dragging->rect.x = mx-100; dragging->rect.y = my-25; }
        renderAll(ren, player, workspace, dragging, isRunning, tNew, resetBtn, startBtn);
        SDL_Delay(10);
    }
    return 0;
}