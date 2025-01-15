#include <iostream>
#include "move.h"
#include "timer.h"
#include "board.h"

static uint64_t countNodes = 0ULL;

float evaluate(Board& board){
	if (board.isCheckmated(WHITE)) return -INFINITY;
	if (board.isCheckmated(BLACK)) return INFINITY;
	if (board.isStalemate()) return 0;	
	countNodes += 1ULL;

	return board.getMaterialCount(WHITE) - board.getMaterialCount(BLACK);
}
float negamax(int depth, Board& board, float alpha, float beta, int colour){

	if (depth == 0)
		return colour * evaluate(board);

	
	float value = -INFINITY;
	
	std::vector<Move> allMoves = board.generateLegalMoves();

	for(auto& move: allMoves){
		board.makeMove(move);
		value = std::max(value, -negamax(depth - 1, board, -beta, -alpha, -colour));
		
		board.unmakeMove();	
		
		alpha = std::max(alpha, value);
		
		if (alpha >= beta)
			break;

	}

	return value;	
}
Move search(Board& board, int colour, int allottedTime, int depth=10){
	Timer timer = Timer();
	Move bestMove;
	float bestValue = -INFINITY;
	float value = -INFINITY;
	float alpha = -INFINITY;
	float beta = INFINITY;

	std::vector<Move> allMoves = board.generateLegalMoves();

	// Return null move
	if (allMoves.size() == 0)
		return Move(NORMAL, H1, H1, KING);

	for (int d = 0; d < depth; i++){
		for (auto& move: allMoves){
			board.makeMove(move);
			value = -negamax(d, board, alpha, beta, -colour);
			board.unmakeMove();
	
			if (value >= bestValue){
				bestMove = move;
				bestValue = value;

			}

			if (timer.getCurrentTime() > allottedTime){
				timer.stop();
				return bestMove;
			}	
		}	
	}
	return bestMove;	

}
