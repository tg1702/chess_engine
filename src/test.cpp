#include <array>

#include "nnue.h"
#include "board.h"

int main(){
    Accumulator side_to_move;
    Accumulator not_side_to_move;

    NNUE nnue;
    Board board = Board("8/8/8/8/4K3/8/8/8 w - - 0 1");

    board.printBoard();

    std::array<int, 768> input = {0};

    board.getPositionIndexes(input);


    for (int i = 0; i < 768; i++){
        if (input[i] == 1)
            std::cout << input[i] << " " << i << '\n';
    }
    

    //nnue.fit();
    return 0;
}
