#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <fstream>
#include <ctime>
#include <iostream>
#include "Block.h"
#include "Sprites.h"
#include <SDL2/SDL_ttf.h>

SDL_Texture* renderText(std::string message, TTF_Font* font, SDL_Color color, SDL_Renderer* ren) {
    SDL_Surface* surf = TTF_RenderText_Blended(font, message.c_str(), color);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, surf);
    SDL_FreeSurface(surf);
    return tex;
}

Uint32 startTime = 0;

const int SCREEN_W = 1200;
const int SCREEN_H = 800;
const int DIV_X = 400;
const int DIV_Y = 400;

struct PaletteItem { ActionType type; SDL_Texture* tex; };
std::vector<PaletteItem> standardItems;
std::vector<PaletteItem> penItems;

int currentPage = 0;
const int BLOCKS_PER_PAGE = 4;
int scrollOffset = 0;
bool showingPenPalette = false;

// Global Background Variables
std::vector<SDL_Texture*> backdrops;
int activeBG = 0;

void logAction(std::string message) {
    std::ofstream logFile("execution_log.txt", std::ios::app);
    time_t now = time(0);
    char* dt = ctime(&now);
    std::string ts(dt);
    if (!ts.empty()) ts.pop_back();
    logFile << "[" << ts << "] " << message << std::endl;
}

void drawTriangle(SDL_Renderer* ren, int x, int y, bool left) {
    SDL_Point points[4];
    if (left) { points[0]={x,y}; points[1]={x+20,y-10}; points[2]={x+20,y+10}; points[3]={x,y}; }
    else      { points[0]={x,y}; points[1]={x-20,y-10}; points[2]={x-20,y+10}; points[3]={x,y}; }
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    SDL_RenderDrawLines(ren, points, 4);
}

void fillCircle(SDL_Renderer* ren, int cx, int cy, int radius) {
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx*dx + dy*dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(ren, cx + dx, cy + dy);
            }
        }
    }
}

// Updated renderAll to accept and draw backgrounds
void renderAll(SDL_Renderer* ren, sprite& player, const std::vector<Block>& workspace, Block* dragging, bool isRunning, SDL_Texture* tNew, SDL_Texture* tExt, SDL_Rect& resetBtn, SDL_Rect& startBtn, SDL_Rect& extBtn, SDL_Texture* canvas, const std::vector<SDL_Texture*>& backdrops, int activeBG) {
    SDL_SetRenderDrawColor(ren, 20, 20, 20, 255);
    SDL_RenderClear(ren);

    // 1. Draw Background First
    if (!backdrops.empty()) {
        SDL_RenderCopy(ren, backdrops[activeBG], NULL, NULL);
    }

    // 2. Draw Pen Canvas over background
    SDL_RenderCopy(ren, canvas, NULL, NULL);

    // 3. Draw UI Sidebars
    SDL_SetRenderDrawColor(ren, 60, 60, 60, 255);
    SDL_Rect sideTop = {0,0, DIV_X, DIV_Y}; SDL_RenderFillRect(ren, &sideTop);
    SDL_SetRenderDrawColor(ren, 40, 40, 40, 255);
    SDL_Rect sideBot = {0, DIV_Y, DIV_X, DIV_Y}; SDL_RenderFillRect(ren, &sideBot);

    SDL_RenderCopy(ren, tNew, NULL, &resetBtn);
    SDL_SetRenderDrawColor(ren, isRunning ? 255 : 0, isRunning ? 0 : 255, 0, 255);
    SDL_RenderFillRect(ren, &startBtn);

    drawTriangle(ren, 370, 365, false);
    drawTriangle(ren, 30, 365, true);

    auto& activePalette = showingPenPalette ? penItems : standardItems;
    for (int i=0; i < BLOCKS_PER_PAGE; i++) {
        int idx = currentPage * BLOCKS_PER_PAGE + i;
        if (idx >= activePalette.size()) break;
        SDL_Rect r = {100, 60 + (i*70), 200, 50};
        SDL_RenderCopy(ren, activePalette[idx].tex, NULL, &r);
    }

    for (auto& b : workspace) {
        SDL_Rect renderRect = {b.rect.x, b.rect.y + scrollOffset, b.rect.w, b.rect.h};
        SDL_RenderCopy(ren, b.texture, NULL, &renderRect);
    }

    renderSprite(player, ren);

    SDL_RenderCopy(ren, tExt, NULL, &extBtn);
    if (dragging) SDL_RenderCopy(ren, dragging->texture, NULL, &dragging->rect);
    SDL_RenderPresent(ren);
}

int main(int argc, char* argv[]) {
    // Clear the log file at launch so it doesn't get massive
    std::ofstream clearLog("execution_log.txt", std::ios::trunc);
    clearLog.close();

    SDL_Init(SDL_INIT_VIDEO); IMG_Init(IMG_INIT_PNG);
    SDL_Window* win = SDL_CreateWindow("Mini Scratch v3 Final", 100, 100, SCREEN_W, SCREEN_H, 0);
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);

    if (TTF_Init() == -1) {
        std::cout << "TTF_Init Error: " << TTF_GetError() << std::endl;
    }
    TTF_Font* font = TTF_OpenFont("arial.ttf", 24);
    startTime = SDL_GetTicks();

    SDL_Texture* canvas = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_W, SCREEN_H);
    SDL_SetTextureBlendMode(canvas, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(ren, canvas);
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 0); SDL_RenderClear(ren);
    SDL_SetRenderTarget(ren, NULL);

    SDL_Texture* tMove = IMG_LoadTexture(ren, "move_btn.png");
    SDL_Texture* tSay = IMG_LoadTexture(ren, "say_btn.png");
    SDL_Texture* tThink = IMG_LoadTexture(ren, "think_btn.png");
    SDL_Texture* tUp = IMG_LoadTexture(ren, "dir_up_btn.png");
    SDL_Texture* tDown = IMG_LoadTexture(ren, "dir_down_btn.png");
    SDL_Texture* tL = IMG_LoadTexture(ren, "dir_left_btn.png");
    SDL_Texture* tR = IMG_LoadTexture(ren, "dir_right_btn.png");
    SDL_Texture* tHide = IMG_LoadTexture(ren, "hide_btn.png");
    SDL_Texture* tShow = IMG_LoadTexture(ren, "show_btn.png");
    SDL_Texture* tRand = IMG_LoadTexture(ren, "random_btn.png");
    SDL_Texture* tMouse = IMG_LoadTexture(ren, "mouse_btn.png");
    SDL_Texture* tHiBub = IMG_LoadTexture(ren, "hi_bubble.png");
    SDL_Texture* tWhyBub = IMG_LoadTexture(ren, "why_bubble.png");
    SDL_Texture* tNew = IMG_LoadTexture(ren, "new_project.png");
    SDL_Texture* tExt = IMG_LoadTexture(ren, "extension_btn.png");

    SDL_Texture* tPD = IMG_LoadTexture(ren, "pen_down_btn.png");
    SDL_Texture* tPU = IMG_LoadTexture(ren, "pen_up_btn.png");
    SDL_Texture* tER = IMG_LoadTexture(ren, "erase_btn.png");
    SDL_Texture* tST = IMG_LoadTexture(ren, "stamp_btn.png");
    SDL_Texture* tPR = IMG_LoadTexture(ren, "pen_red_btn.png");
    SDL_Texture* tPB = IMG_LoadTexture(ren, "pen_blue_btn.png");
    SDL_Texture* tSU = IMG_LoadTexture(ren, "pen_size_up.png");
    SDL_Texture* tSD = IMG_LoadTexture(ren, "pen_size_down.png");

    SDL_Texture* tSizeInc = IMG_LoadTexture(ren, "size_inc.png");
    SDL_Texture* tSizeDec = IMG_LoadTexture(ren, "size_dec.png");
    SDL_Texture* tSizeRes = IMG_LoadTexture(ren, "size_res.png");
    SDL_Texture* tWait = IMG_LoadTexture(ren, "wait_btn.png");
    SDL_Texture* tTRes = IMG_LoadTexture(ren, "timer_res.png");
    SDL_Texture* tTSay = IMG_LoadTexture(ren, "timer_say.png");

    // NEW TEXTURES
    SDL_Texture* tRepStart = IMG_LoadTexture(ren, "repeat_start.png");
    SDL_Texture* tRepEnd = IMG_LoadTexture(ren, "repeat_end.png");
    SDL_Texture* tNextBG = IMG_LoadTexture(ren, "next_bg_btn.png");

    backdrops.push_back(IMG_LoadTexture(ren, "void.png")) ;
    backdrops.push_back(IMG_LoadTexture(ren, "beach.png"));
    backdrops.push_back(IMG_LoadTexture(ren, "field.png"));


    standardItems = {

            {MOVE, tMove},
            {SET_UP, tUp},
            {SET_DOWN, tDown},
            {SET_LEFT, tL},
            {SET_RIGHT, tR},
            {GOTO_RANDOM, tRand},
            {GOTO_MOUSE, tMouse},


            {SAY, tSay},
            {THINK, tThink},
            {HIDE, tHide},
            {SHOW, tShow},
            {SIZE_INC, tSizeInc},
            {SIZE_DEC, tSizeDec},
            {SIZE_RESET, tSizeRes},
            {NEXT_BG, tNextBG},


            {WAIT_SEC, tWait},
            {REPEAT_BEGIN, tRepStart},
            {REPEAT_END, tRepEnd},
            {TIMER_RESET, tTRes},
            {TIMER_SAY, tTSay}
    };

    penItems = { {PEN_DOWN,tPD}, {PEN_UP,tPU}, {ERASE_ALL,tER}, {STAMP,tST}, {SET_PEN_RED,tPR}, {SET_PEN_BLUE,tPB}, {PEN_SIZE_INC,tSU}, {PEN_SIZE_DEC,tSD} };

    sprite player; initSprite(player, 700, 400);
    std::vector<Block> workspace;
    Block* dragging = nullptr;

    SDL_Rect nextBtn={350,350,30,30}, prevBtn={20,350,30,30}, startBtn={550,20,50,50}, resetBtn={20,20,100,30}, extBtn={150,355,100,30};
    bool isRunning = false, quit = false;
    SDL_Event e;

    while (!quit) {
        int mx, my; SDL_GetMouseState(&mx, &my);
        auto& activePalette = showingPenPalette ? penItems : standardItems;

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
            if (e.type == SDL_MOUSEWHEEL && mx < DIV_X && my > DIV_Y) scrollOffset += e.wheel.y * 20;

            if (e.type == SDL_MOUSEBUTTONDOWN && !isRunning) {
                SDL_Point p = {mx, my};

                if (SDL_PointInRect(&p, &resetBtn)) {
                    workspace.clear(); initSprite(player, 700, 400); scrollOffset = 0;
                    SDL_SetRenderTarget(ren, canvas); SDL_SetRenderDrawColor(ren, 0, 0, 0, 0); SDL_RenderClear(ren); SDL_SetRenderTarget(ren, NULL);
                }

                if (SDL_PointInRect(&p, &startBtn)) {
                    int loopDepth = 0;
                    bool isValid = true;
                    for (size_t i = 0; i < workspace.size(); i++) {
                        if (workspace[i].action == REPEAT_BEGIN) loopDepth++;
                        else if (workspace[i].action == REPEAT_END) loopDepth--;

                        if (loopDepth < 0) {
                            logAction("[ERROR] Validation Failed: Repeat End without Begin at index " + std::to_string(i));
                            std::cout << "Syntax Error: Repeat End without Begin!" << std::endl;
                            isValid = false; break;
                        }
                    }
                    if (isValid && loopDepth > 0) {
                        logAction("[ERROR] Validation Failed: Missing Repeat End block.");
                        std::cout << "Syntax Error: Missing Repeat End block!" << std::endl;
                        isValid = false;
                    }

                    if (isValid) isRunning = true;
                }

                if (SDL_PointInRect(&p, &extBtn)) { showingPenPalette = !showingPenPalette; currentPage = 0; }
                if (SDL_PointInRect(&p, &nextBtn) && (currentPage+1)*BLOCKS_PER_PAGE < activePalette.size()) currentPage++;
                if (SDL_PointInRect(&p, &prevBtn) && currentPage > 0) currentPage--;

                if (e.button.button == SDL_BUTTON_RIGHT) {
                    for (size_t i = 0; i < workspace.size(); i++) {
                        SDL_Rect r = {workspace[i].rect.x, workspace[i].rect.y + scrollOffset, workspace[i].rect.w, workspace[i].rect.h};
                        if (SDL_PointInRect(&p, &r)) { workspace.erase(workspace.begin() + i); break; }
                    }
                }

                for (int i=0; i < BLOCKS_PER_PAGE; i++) {
                    int idx = currentPage * BLOCKS_PER_PAGE + i;
                    if (idx >= activePalette.size()) break;
                    SDL_Rect r = {100, 60 + (i*70), 200, 50};
                    if (SDL_PointInRect(&p, &r)) dragging = new Block(createBlock(mx-100, my-25, 200, 50, activePalette[idx].type, activePalette[idx].tex));
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
            logAction("--- Start Execution ---");

            int pc = 0;
            std::vector<int> loopStack;
            std::vector<int> loopCounts;

            while (pc < workspace.size() && isRunning) {
                Block& b = workspace[pc];

                SDL_Event execEv;
                while (SDL_PollEvent(&execEv)) {
                    if (execEv.type == SDL_QUIT) { quit = true; isRunning = false; }
                }
                if (!isRunning) break;

                if (b.action == REPEAT_BEGIN) {
                    if (loopStack.empty() || loopStack.back() != pc) {
                        loopStack.push_back(pc);
                        loopCounts.push_back(4);
                        logAction("Entered Loop. Iterations set to 4.");
                    }
                }
                else if (b.action == REPEAT_END) {
                    if (!loopStack.empty()) {
                        if (loopCounts.back() > 1) {
                            loopCounts.back()--;
                            pc = loopStack.back();
                            logAction("Looping back. Remaining: " + std::to_string(loopCounts.back()));
                        } else {
                            loopStack.pop_back();
                            loopCounts.pop_back();
                            logAction("Exited Loop.");
                        }
                    }
                }
                else if (b.action == MOVE) {
                    for(int step=0; step<10; step++) {
                        if (player.dir == RIGHT) player.rect.x += 3;
                        else if (player.dir == LEFT)  player.rect.x -= 3;
                        else if (player.dir == UP)    player.rect.y -= 3;
                        else if (player.dir == DOWN)  player.rect.y += 3;

                        if (player.rect.x < DIV_X) { player.rect.x = DIV_X; player.dir = RIGHT; }
                        else if (player.rect.x > SCREEN_W-80) { player.rect.x = SCREEN_W-80; player.dir = LEFT; }
                        else if (player.rect.y < 0) { player.rect.y = 0; player.dir = DOWN; }
                        else if (player.rect.y > SCREEN_H-80) { player.rect.y = SCREEN_H-80; player.dir = UP; }

                        if (player.isPenDown) {
                            SDL_SetRenderTarget(ren, canvas);
                            SDL_SetRenderDrawColor(ren, player.penColor.r, player.penColor.g, player.penColor.b, 255);
                            fillCircle(ren, player.rect.x + 35, player.rect.y + 35, player.penSize);
                            SDL_SetRenderTarget(ren, NULL);
                        }
                        // UPDATED CALL
                        renderAll(ren, player, workspace, nullptr, true, tNew, tExt, resetBtn, startBtn, extBtn, canvas, backdrops, activeBG);
                        SDL_Delay(50);
                    }
                }
                else {
                    if (b.action == SAY) { player.activeBubble = tHiBub; logAction("Looks Report: Sprite said 'Hi'"); }
                    else if (b.action == THINK) { player.activeBubble = tWhyBub; logAction("Looks Report: Sprite thought '...'"); }
                    else if (b.action == HIDE) { player.alpha = 0; logAction("Looks Report: Sprite hidden"); }
                    else if (b.action == SHOW) { player.alpha = 255; logAction("Looks Report: Sprite shown"); }
                    else if (b.action == SET_UP) player.dir = UP;
                    else if (b.action == SET_DOWN) player.dir = DOWN;
                    else if (b.action == SET_LEFT) player.dir = LEFT;
                    else if (b.action == NEXT_BG) {
                        activeBG = (activeBG + 1) % backdrops.size();
                        logAction("Looks Report: Background changed to index " + std::to_string(activeBG));
                    }
                    else if (b.action == SIZE_INC) {
                        player.scale += 0.2f;
                        logAction("Looks Report: Sprite size increased. Scale: " + std::to_string(player.scale));
                    }
                    else if (b.action == SIZE_DEC) {
                        if (player.scale > 0.3f) player.scale -= 0.2f;
                        logAction("Looks Report: Sprite size decreased. Scale: " + std::to_string(player.scale));
                    }
                    else if (b.action == SIZE_RESET) {
                        player.scale = 1.0f;
                        logAction("Looks Report: Sprite size reset to 100%.");
                    }
                    else if (b.action == WAIT_SEC) {
                        logAction("Waiting for 1 second...");
                        // UPDATED CALL
                        renderAll(ren, player, workspace, nullptr, true, tNew, tExt, resetBtn, startBtn, extBtn, canvas, backdrops, activeBG);
                        SDL_Delay(1000);
                    }
                    else if (b.action == TIMER_RESET) {
                        startTime = SDL_GetTicks();
                        logAction("Timer Reset to 0.");
                    }
                    else if (b.action == TIMER_SAY) {
                        Uint32 elapsed = (SDL_GetTicks() - startTime) / 1000;
                        std::string timeStr = "Time: " + std::to_string(elapsed) + "s";
                        SDL_Color white = {255, 255, 255, 255};
                        player.activeBubble = renderText(timeStr, font, white, ren);
                        logAction("Sprite reported the timer: " + timeStr);
                    }
                    else if (b.action == SET_RIGHT) player.dir = RIGHT;
                    else if (b.action == GOTO_RANDOM) {
                        player.rect.x = (rand() % (SCREEN_W - DIV_X - 80)) + DIV_X;
                        player.rect.y = rand() % (SCREEN_H - 80);
                    }
                    else if (b.action == GOTO_MOUSE) {
                        bool clickedOnStage = false;
                        logAction("Waiting for stage click...");
                        while (!clickedOnStage) {
                            SDL_Event clickEv;
                            while (SDL_PollEvent(&clickEv)) {
                                if (clickEv.type == SDL_QUIT) { quit = true; clickedOnStage = true; isRunning = false; }
                                if (clickEv.type == SDL_MOUSEBUTTONDOWN) {
                                    int cx, cy; SDL_GetMouseState(&cx, &cy);
                                    if (cx > DIV_X) {
                                        player.rect.x = cx - 40; player.rect.y = cy - 40;
                                        if (player.rect.x > SCREEN_W - 80) player.rect.x = SCREEN_W - 80;
                                        if (player.rect.y < 0) player.rect.y = 0;
                                        if (player.rect.y > SCREEN_H - 80) player.rect.y = SCREEN_H - 80;
                                        clickedOnStage = true;
                                    }
                                }
                            }
                            // UPDATED CALL
                            renderAll(ren, player, workspace, nullptr, true, tNew, tExt, resetBtn, startBtn, extBtn, canvas, backdrops, activeBG);
                            SDL_Delay(10);
                        }
                    }
                    else if (b.action == PEN_DOWN) player.isPenDown = true;
                    else if (b.action == PEN_UP)   player.isPenDown = false;
                    else if (b.action == SET_PEN_RED) player.penColor = {255, 0, 0, 255};
                    else if (b.action == SET_PEN_BLUE) player.penColor = {0, 0, 255, 255};
                    else if (b.action == PEN_SIZE_INC && player.penSize < 40) player.penSize += 4;
                    else if (b.action == PEN_SIZE_DEC && player.penSize > 2) player.penSize -= 4;
                    else if (b.action == STAMP) {
                        SDL_SetRenderTarget(ren, canvas); renderSprite(player, ren); SDL_SetRenderTarget(ren, NULL);
                    }
                    else if (b.action == ERASE_ALL) {
                        SDL_SetRenderTarget(ren, canvas); SDL_SetRenderDrawColor(ren, 0, 0, 0, 0); SDL_RenderClear(ren); SDL_SetRenderTarget(ren, NULL);
                    }

                    // UPDATED CALL
                    renderAll(ren, player, workspace, nullptr, true, tNew, tExt, resetBtn, startBtn, extBtn, canvas, backdrops, activeBG);
                    SDL_Delay(200);
                }
                pc++;
            }
            isRunning = false;
            logAction("--- End Execution ---");
        }

        if (dragging) { dragging->rect.x = mx-100; dragging->rect.y = my-25; }
        // UPDATED CALL
        renderAll(ren, player, workspace, dragging, isRunning, tNew, tExt, resetBtn, startBtn, extBtn, canvas, backdrops, activeBG);
        SDL_Delay(10);
    }

    TTF_CloseFont(font);
    TTF_Quit();

    return 0;
}