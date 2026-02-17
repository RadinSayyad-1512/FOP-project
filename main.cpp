#include <iostream>
#include "blocks.h"
#include "Executor.h"
#include <vector>
int main() {
    std::vector <Block> program ;
    //some random trial block
    Block move ,repeat , say ;
    move.type = BlockType::MOVE;
    move.numberValue = 10 ;
    //10 steps

    repeat.type = BlockType::REPEAT_BLOCK ;
    repeat.numberValue = 3 ;
    //3 repeats

    say.type = BlockType::SAY ;
    say.textValue = "Hello" ;
    //says hello

    program.push_back(move) ;
    program.push_back(repeat) ;
    program.push_back(say) ;

    for ( int i = 0 ; i < program.size() ; i++ ) {
        executeBlock(program , i ) ;
    }
    return 0 ;
}