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


float evaluate(Board& board){
    if (board.isCheckmated(WHITE)) return -INFINITY;
    if (board.isCheckmated(BLACK)) return INFINITY;
    if (board.isDraw()) return 0;    

    return board.getMaterialCount(WHITE) - board.getMaterialCount(BLACK);
}

float negamax(int depth, Board& board, float alpha, float beta, int colour){
    if (depth == 0 || !isSearching)
        return colour * evaluate(board);

    float value = -INFINITY;
    
    std::vector<Move> allMoves = board.generateLegalMoves();
    
    for(auto& move: allMoves){
        board.makeMove(move);
		value = std::max(value, -negamax(depth - 1, board, -beta, -alpha, -colour));
        board.unmakeMove();    

        
        alpha = std::max(alpha, value);
        
       	if (alpha >= beta){
            break; // Beta cutoff
        } 
            
    }
    return value;    
}

void search(Board& board, int colour, int allottedTime, int depth=10){
    Timer timer = Timer();
    Move bestMove{Move(NORMAL, H1, H1, KING)};
	

	std::vector<Move> allMoves = board.generateLegalMoves();  
    

    for (int d = 1; d <= depth && isSearching; d++){  // Start from depth 1
        float alpha = -INFINITY;
        float beta = INFINITY;
		float bestIterationValue = -INFINITY;
		Move bestIterationMove = allMoves[0];
        


        for (auto& move: allMoves){
            board.makeMove(move);
            float value = -negamax(d - 1, board, -beta, -alpha, -colour);
            
            board.unmakeMove();

			if (value > bestIterationValue){
                bestIterationMove = move;
                bestIterationValue = value;

            }
            
           
            


            
            if (timer.getCurrentTime() > allottedTime || !isSearching){
                timer.stop();
                std::cout << "bestmove " << bestMove << '\n';
                isSearching.store(false);
                return;
            }    
        }

		bestMove = bestIterationMove;

    }
    isSearching.store(false);
    std::cout << "bestmove " << bestMove << '\n';
}
