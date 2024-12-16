#include <iostream>
#include <array>
#include "../src/board.h"
#include "../src/move.h"
#include "../src/types.h"

#define DEPTH 5

uint64_t perft(int depth, Board* board){
	
	uint64_t nodes = 0ULL;
	int index, move_count;

	uint64_t level_count = 0ULL;

	if (depth == 0)
	{
		//board->printBoard();	
		return 1ULL;
	}

	std::vector<Move> move_list = board->generateMoves();	
	bool turn = board->getTurn();

	for(const auto& move: move_list){		
		uint32_t from = move.getFrom();
		uint32_t to = move.getTo();
		uint32_t flags = move.getFlag();

		board->makeMove(from, to, flags);
		
		if (!board->isInCheck(turn)){	
			level_count += perft(depth - 1, board);		
			nodes += level_count;
		}	
		
		if (depth == DEPTH){
			cout << pieceSquareNames[from] << pieceSquareNames[to] << ": " << level_count << endl;
			level_count = 0;
		}

		
		board->unmakeMove();	
	}
	
	return level_count;
}

int main(){
	Board* board = new Board();
/*	
	board->generateMoves();
	board->makeMove(E2, E4);
	board->printBoard();
	board->generateMoves();
	board->makeMove(C7, C5);
	board->printBoard();
	board->generateMoves();
	board->makeMove(E4, E5);
	board->printBoard();
	board->generateMoves();
	board->makeMove(D7, D5);
	board->printBoard();
	board->generateMoves();
	board->makeMove(E5,D6);
	board->printBoard();
	board->unmakeMove();
	board->unmakeMove();
	board->unmakeMove();
	board->printBoard();
*/
	
	std::cout << perft(DEPTH, board) << endl;
	return 0;
	delete board;
}
