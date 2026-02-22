//
// Created by Amirmohammad Mirzaei on 2/18/2026.
//

#include "Executor.h"
#include <iostream>

void executeProgram(sprite &s, SDL_Renderer* ren, SDL_Texture* page) {
    int repeatCount = 5;
    int loopStartPC;
 for (int pc = 0; pc < s.myProgram.size(); pc++) {
     Block& b = s.myProgram[pc];

     if (b.action == REPEAT_BEGIN) {
         loopStartPC = pc;
     }

     else if (b.action == REPEAT_END) {
         if (repeatCount > 1) {
             repeatCount--;
             pc = loopStartPC;
             std:: cout << "Looping back... Remaining:" << repeatCount << std :: endl;
         }
     }

     if (b.action == MOVE) {
         if (s.dir == UP) s.rect.y -= 20;
         else if (s.dir == DOWN) s.rect.y += 20;
         else if (s.dir == LEFT) s.rect.x - +20;
         else if (s.dir == RIGHT) s.rect.x += 20;
     }

     else if (b.action == GHOST_UP) {
         if (s.alpha >= 51) s.alpha -= 51;
     }
     else if (b.action == TURN_RIGHT)

         std:: cout << "Executing block: " << pc << std :: endl;

     //Short pause for movement to be seen
     SDL_Delay(150);
 }
}
