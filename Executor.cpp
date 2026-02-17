#include "Executor.h"
#include "sprites.h"
#include <cmath>

#define PI 3.141592

void executeBlock(std::vector<Block>& program, int& index, sprite& sprite, int screenWidth, int screenHeight)
{
    Block& block = program[index];

    switch(block.type)
    {
        case BlockType::MOVE:
        {
            float dx = block.numberValue * cos(sprite.directionDegree * PI / 180.0f);
            float dy = block.numberValue * sin(sprite.directionDegree * PI / 180.0f);

            if(sprite.validateMove(dx, dy, screenWidth, screenHeight)) {
                sprite.x += (float )dx;
                sprite.y += (float )dy;
            }
            break;
        }

        case BlockType::TURN:
        {
            sprite.directionDegree += block.numberValue;
            if(sprite.directionDegree >= 360) sprite.directionDegree -= 360;
            if(sprite.directionDegree < 0) sprite.directionDegree += 360;
            break;
        }

        case BlockType::REPEAT_BLOCK:
        {
            int repeatCount = (int)block.numberValue;
            if(index + 1 < program.size()) {
                for(int i=0;i<repeatCount;i++) {
                    int nextIndex = index+1;
                    executeBlock(program, nextIndex, sprite, screenWidth, screenHeight);
                }
            }
            break;
        }

/*        case BlockType::TOUCHING:
        {

            if(sprite.x < 500 && index + 1 < program.size()) {
                int nextIndex = index+1;
                executeBlock(program, nextIndex, sprite, screenWidth, screenHeight);
            }
            break;
        }
*/
        default:
            break;
    }
}
