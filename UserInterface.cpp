//
// Created by Amirmohammad Mirzaei on 2/22/2026.
//

#include "UserInterface.h"

extern bool isRunning;

void drawInfoBox(SDL_Renderer* ren , const sprite& s) {
    int infoBoxX = 800 - 220;
    int infoBoxY = 20;
    SDL_Rect box = {infoBoxX, infoBoxY, 200, 100};

//Drawing of a glass body
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(ren, 40, 40, 40, 200);
    SDL_RenderFillRect(ren, &box);

    if (isRunning) {
        SDL_SetRenderDrawColor(ren, 255, 255, 0, 255);
    }
    else {
        SDL_SetRenderDrawColor(ren, 77, 151, 255, 255);

    }


    int barWidth = (s.rect.x / 4);
    if (barWidth > 180) barWidth = 180;

    SDL_Rect xBar = {infoBoxX + 10, infoBoxY + 40, (s.rect.x / 4), 10};
    SDL_SetRenderDrawColor(ren, 255, 100, 100, 255);
    SDL_RenderFillRect(ren, &xBar);


  // Draw a bright border
  int alphaWidth = (s.alpha * 180 ) / 255;
  SDL_Rect  aBar = {infoBoxX + 10, infoBoxY + 70, alphaWidth, 10};
    SDL_SetRenderDrawColor(ren, 0, 200 ,255, 255);
    SDL_RenderDrawRect(ren, &box);



    }


