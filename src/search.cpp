#include <iostream>
#include "move.h"
#include "board.h"

#define INFINITY 99999999
float evaluate(Board& board){
	if (board.isCheckmated(WHITE)) return -INFINITY;
	if (board.isCheckmated(BLACK)) return INFINITY;
	
	return board.getMaterialCount(WHITE) - board.getMaterialCount(BLACK);
}
float negamax(int depth, Board& board, float alpha, float beta, int colour){
	if (depth == 0 || board.isGameOver())
		return colour * evaluate(board);

	
	float value = -INFINITY;
	
	std::vector<Move> allMoves = board.generateLegalMoves();

	for(auto& move: allMoves){
		value = std::max(value, -negamax(depth - 1, board, -beta, -alpha, -colour));
		alpha = std::max(alpha, value);
		
		if (alpha >= beta)
			break;	
	}

	return value;	
}
Move search(int depth, Board& board, int colour){

	Move bestMove;
	float bestValue = -INFINITY;
	float value = -INFINITY;
	float alpha = -INFINITY;
	float beta = INFINITY;

	std::vector<Move> allMoves = board.generateLegalMoves();

	for (auto& move: allMoves){
		board.makeMove(move);
		value = -negamax(depth, board, alpha, beta, -colour);
		board.unmakeMove();

		if (value > bestValue){
			bestMove = move;
			bestValue = value;

		}	
	}

	return bestMove;	

}


int main(){
	Board board = Board("rnbqkbnr/pppp1ppp/8/4p3/5PP1/8/PPPPP2P/RNBQKBNR b KQkq - 0 2");
	Move resultMove = search(6, board, -1);
	std::cout << "bestmove " << resultMove << '\n';
	return 0;
}
