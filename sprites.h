//
// Created by parsa on 2/17/2026.
//

#ifndef FOP_ENGINE_SPRITES_H
#define FOP_ENGINE_SPRITES_H
#include <string>
struct sprite {
    float x = 0 , y = 0 ;
    float w = 50 , h = 50 ;
    int index ;
    bool shown = false ;
    float dy = 0 , dx = 0 ;
    float directionDegree = 0 ;
    std::string speechText , thinkText ;
    bool validateMove ( float deltaX , float deltaY , int screenW , int screenH ) {
        float nx = x + deltaX ;
        float ny = y + deltaY ;
        return nx >= 0 && nx <= screenW && ny >= 0 && ny <= screenH ;
    }
};

#endif //FOP_ENGINE_SPRITES_H
