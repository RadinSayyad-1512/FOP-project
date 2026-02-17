//
// Created by parsa on 2/17/2026.
//

#ifndef FOP_ENGINE_BLOCKS_H
#define FOP_ENGINE_BLOCKS_H
#include <memory>
#include <vector>
#include <string>

enum class BlockType {
    //events
    WHEN_GREEN_FLAG,

    //MOTION
    MOVE,
    TURN,
    SET_X,
    SET_Y,
    CHANGE_X,
    CHANGE_Y,
    GOTO_RANDOM,
    GOTO_MOUSE,

    //LOOKS
    SAY,
    THINK,
    SET_SIZE,
    CHANGE_SIZE,
    HIDE,
    SHOW,
    SWITCH_TO_COSTUME,
    NEXT_COSTUME ,
    PREV_COSTUME,
    SWITCH_TO_BG,
    NEXT_BG ,
    PREV_BG,

    //SOUND
    PLAY_SOUND,
    STOP_ALL_SOUNDS,
    SET_VOLUME,
    CHANGE_VOLUME,
    SET_PITCH,
    CHANGE_PITCH,

    //CONTROL
    IF_BLOCK,
    REPEAT_BLOCK,
    WAIT,

    //SENSE
    TOUCHING,
    DISTANCE,
    MOUSE_X , MOUSE_Y, MOUSE_DOWN ,

    //PEN
    PEN_DOWN,
    PEN_UP,
    PEN_SET_COLOR,
    PEN_SET_SIZE,
    PEN_CLEAR,
    PEN_STAMP,

    //VARIABLES
    SET_VAR,
    CHANGE_VAR,

    //13 MAIN OPERATIONS
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    GT,
    LT,
    EQ,
    AND,
    OR,
    NOT,
    RANDOM,
    MOD,
    ROUND,

    //8 EXTRA OPPS
    ABS,
    FLOOR,
    CEIL,
    MIN,
    MAX,
    LENGTH,
    CONTAINS,
    JOIN,

};

struct Block {
    BlockType type ;
    int numberValue = 0 ;
    std::string textValue ;
    std::string varName;
};



#endif //FOP_ENGINE_BLOCKS_H
