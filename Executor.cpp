//
// Created by Amirmohammad Mirzaei on 2/18/2026.
//

#include "Executor.h"
#include <iostream>

void executeProgram(sprite &s, SDL_Renderer* ren, SDL_Texture* page) {
 for (int i = 0; i < s.myProgram.size(); i++) {
     Block& b = s.myProgram[i];

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

         std:: cout << "Executing block: " << i << std :: endl;

     //Short pause for movement to be seen
     SDL_Delay(150);
 }
}
