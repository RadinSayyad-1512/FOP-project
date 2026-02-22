//
// Created by Mirzaei on 2/22/2026.
//
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "SoundManager.h"
#include <iostream>

    Mix_Chunk *popSound = nullptr;

bool initAudio(){

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std:: cout << "SDL_mixer could not intialize! Error:" << Mix_GetError() << std:: endl;
    }



    popSound = Mix_LoadWAV ("pop.wav");

  if (  popSound ==nullptr)
  {
      std:: cout << "Failed to load pop.wav! Error:" << Mix_GetError() << std:: endl;
      return false;
  }

    std:: cout << "Audio intialized and pop.wav loaded successfully! Error:" << Mix_GetError() << std:: endl;
    return true;


}

void playPop() {
    if (popSound != nullptr) {
        Mix_PlayChannel(-1, popSound, 0);
    }
    else {
        std:: cout << "Sound is null, cannor play" << std::endl;
    }
}


void closeAudio() {
    if (popSound != nullptr) {

        Mix_FreeChunk(popSound);
        popSound = nullptr;
    }
    Mix_CloseAudio();
}