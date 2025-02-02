#include <iostream>
#include <array>
#include <chrono>
#include <climits>
#include <thread>
#include <atomic>
#include <cmath>
#include <mutex>

#include "board.h"
#include "move.h"
#include "search.h"
#include "timer.h"

std::atomic<bool> isSearching{false};
std::mutex inputMutex;




float evaluate(Board& board){
	if (board.isCheckmated(WHITE)) return -INFINITY;
	if (board.isCheckmated(BLACK)) return INFINITY;
	//if (board.isDraw()) return 0;	

	//board.printBoard();
	//std::cout << "white material " << board.getMaterialCount(WHITE) << '\n';
	//std::cout << "black material " << board.getMaterialCount(BLACK) << '\n';

	return board.getMaterialCount(WHITE) - board.getMaterialCount(BLACK);
}
float negamax(int depth, Board& board, float alpha, float beta, int colour){

	if (depth == 0 || !isSearching )
		return colour * evaluate(board);

	
	float value = -INFINITY;
	
	std::vector<Move> allMoves = board.generateLegalMoves();

	for(auto& move: allMoves){
		board.makeMove(move);
		value = std::max(value, -negamax(depth - 1, board, -beta, -alpha, -colour));


		
		board.unmakeMove();	
		
	//	alpha = std::max(alpha, value);
		
		//if (alpha >= beta)
			//break;

	}

	return value;	
}
void search(Board& board, int colour, int allottedTime, int depth=10){


	
	Move bestMove{Move(NORMAL, H1, H1, KING)};


	std::vector<Move> allMoves = board.generateLegalMoves();
	
	// Return null move
	if (allMoves.size() == 0){
		std::cout << "bestmove " << bestMove << '\n';

		isSearching.store(false);
		return;
	}

	for (int d = 1; d <= depth && isSearching; d++){
				
		float alpha = -INFINITY;
		float beta = INFINITY;

		Timer timer = Timer();
		float bestValue = -INFINITY;
		float value = -INFINITY;
		
		for (auto& move: allMoves){
			board.makeMove(move);
			value = negamax(d, board, alpha, beta, colour);
			
			board.unmakeMove();
	
			if (value >= bestValue){
				bestMove = move;
				bestValue = value;
				
		//		std::cout << " new best " << bestMove << '\n';
//				std::cout << "eval " << value << '\n';
			}

			if (timer.getCurrentTime() > allottedTime || !isSearching){
				timer.stop();
				std::cout << "bestmove " << bestMove << '\n';
				return;	
			}	
		}
	

	}
	
	isSearching.store(false);
	std::cout << "bestmove " << bestMove << '\n';
}
