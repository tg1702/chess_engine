#include <iostream>
#include <array>
#include "../src/board.h"
#include "../src/move.h"
#include "../src/types.h"

#define DEPTH 6

uint64_t divide(int depth, Board* board){
	
	uint64_t nodes = 0ULL;

	uint64_t level_count = 0ULL;

	if (depth == 0)
	{
	//	board->printBoard();
		return 1ULL;
	}

	MoveList list = board->generateMoves();

	std::array<Move, 256> move_list = (list).getMoves();	
	int count = board->getMoveCount();
	bool turn = board->getTurn();
		
	for(size_t i = 0; i < count; ++i){			
		int from = move_list[i].getFrom();
		int to = move_list[i].getTo();
		int flag = move_list[i].getFlag();

		board->makeMove(move_list[i]);
		
		if (!board->isInCheck(turn)){	
			level_count += divide(depth - 1, board);		
			nodes += level_count;
		}	
		
		if (depth == DEPTH){
			cout << pieceSquareNames[from] << pieceSquareNames[to] << ": " << level_count << endl;
			//board->printBoard();
			
			level_count = 0;
		}

		
		board->unmakeMove();	
	}
	
	return level_count;
}


uint64_t perft(int depth, Board* board){
	
	uint64_t nodes = 0ULL;

	if (depth == 0)
	{
		return 1ULL;
	}

	MoveList list = board->generateMoves();

	std::array<Move, 256> move_list = (list).getMoves();	
	int count = board->getMoveCount();
	bool turn = board->getTurn();
		
	for(size_t i = 0; i < count; ++i){			

			
		int from = move_list[i].getFrom();
		int to = move_list[i].getTo();
		int flag = move_list[i].getFlag();

		board->makeMove(move_list[i]);
		
		if (!board->isInCheck(turn)){	
			nodes += perft(depth - 1, board);		
		}		
		
		board->unmakeMove();	
	}
	
	return nodes;
}
int main(){
	Board* board = new Board();
/*	board->generateMoves();
	board->makeMove(E2, E4);
	board->printBoard();
	board->generateMoves();
	board->makeMove(D7, D5);
	board->printBoard();
	board->generateMoves();
	board->makeMove(F2, F4);
	board->printBoard();
	board->generateMoves();
	board->makeMove(D5, D4);
	board->printBoard();
	board->generateMoves();
	board->makeMove(E4, E5);
	board->printBoard();
	//board->generateMoves();
	//board->makeMove(B8, C6);
	//board->printBoard();	
*/	
	//std::cout << divide(DEPTH, board) << endl;
	
	std::cout << perft(DEPTH, board) << endl;
	delete board;
	return 0;
}
