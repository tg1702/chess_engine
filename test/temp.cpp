#include <iostream>
#include "../src/board.h"
#include <array>

int main(){
	array<unsigned int, 256> moves;
	Board board = Board();

	board.printBoard();
	board.makeMove(E2, E4, WHITE);
	board.printBoard();
	board.makeMove(B1, C3, WHITE);
	board.printBoard();
	board.makeMove(E7, E5, BLACK);
	board.printBoard();
	cout << endl << endl;
	board.printBoard();	
	moves = board.getMove();

	for (const auto& move: moves){
		cout << "start: " << ((move >> 6) & 0x3f) << "  end: "<< (move & 0x3f) << endl;
	}
	return 0;
}
