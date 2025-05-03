#include <iostream>
#include <array>
#include <chrono>
#include <climits>
#include <thread>
#include <atomic>
#include <cmath>
#include <mutex>
#include <random>

#include "../board.h"
#include "../movegen/move.h"
#include "search.h"
#include "../uci/timer.h"
#include "pst.h"
#include "../types.h"

std::atomic<bool> isSearching{false};
std::mt19937 mt{std::random_device{}()};
std::uniform_real_distribution<double> distribution(0.0,1.0);

float evaluate(Board& board){
    int w_mg_score = 0;
    int b_mg_score = 0;
    int w_eg_score = 0;
    int b_eg_score = 0;
    int gamePhase = 0;

    bool turn = board.getTurn();

   /*  std::cout << "------------------------------------------------------" << '\n'; */

    for (Square sq: Squares){
        for (PieceType pc: PieceTypes){
            if (board.getBitboard(WHITE, pc) & bitset(sq)){

                /* if (pc == ROOK && turn == WHITE){
                    std::cout << "mat value and pesto " << MaterialValues[pc] << ", " << mg_pesto_table[pc][sq] << '\n';
                } */


                w_mg_score += MaterialValues[pc] + mg_pesto_table[pc][sq];
                /* std::cout << "current score: " <<  MaterialValues[pc] + mg_pesto_table[pc][sq] << ",score: " << w_mg_score << ", piece_type: " << pc << '\n';

                std::cout << "square " << sq << '\n'; */
                w_eg_score += MaterialValues[pc] + eg_pesto_table[pc][sq];
                gamePhase += gamePhaseInc[pc];
            }
                
            
            else if (board.getBitboard(BLACK, pc) & bitset(sq)){

                /* if (pc == ROOK){
                    std::cout << "mat value and pesto " << MaterialValues[pc] << ", " << mg_pesto_table[pc][63-sq] << '\n';
                    std::cout << "square " << sq << '\n';
                } */

                b_mg_score += MaterialValues[pc] + mg_pesto_table[pc][56 ^ sq];

                /* std::cout << "side " << BLACK << "current score: " <<  MaterialValues[pc] + mg_pesto_table[pc][56 ^ sq] << ",score: " << b_mg_score << ", piece_type: " << pc << '\n';
                std::cout << "square " << sq << '\n'; */
                b_eg_score += MaterialValues[pc] + eg_pesto_table[pc][56 ^ sq];
                gamePhase += gamePhaseInc[pc];
            }
                

        }
    }
/* 
    std::cout << "------------------------------------------------------" << '\n';

    std::cout << " white mg " << w_mg_score << '\n';
    std::cout << " black mg " << b_mg_score << '\n'; */
    //board.printBoard();

    int mgScore = w_mg_score - b_mg_score;
    int egScore = w_eg_score - b_eg_score;

    int mgPhase = gamePhase;
    if (mgPhase > 24) mgPhase = 24; /* in case of early promotion */
    int egPhase = 24 - mgPhase;
    return (mgScore * mgPhase + egScore * egPhase) / 24;

    //return w_mg_score - b_mg_score;
    //return (board.getMaterialCount(WHITE) - board.getMaterialCount(BLACK)) + distribution(mt);
}

float negamax(int depth, Board& board, float alpha, float beta, int colour){
    std::vector<Move> allMoves = board.generateLegalMoves();

    if (board.isCheckmated(WHITE)) return colour * -INFINITY;
    if (board.isCheckmated(BLACK)) return colour * INFINITY;
    if (board.isDraw()) return 0;    

    if (depth == 0 || !isSearching)
        return colour * evaluate(board);

    float value = -INFINITY;
    
    
    
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
	float bestValue = -INFINITY;
    int lastDepth = 0;

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
               // std::cout << "bestvalue " << bestIterationValue << '\n';

            }
            
           
            


            
            if (timer.getCurrentTime() > allottedTime || !isSearching){
                timer.stop();
                std::cout << "bestmove " << bestMove << '\n';
                std::cout << "info score cp " << bestValue << " depth " << lastDepth << "\n";
                isSearching.store(false);
                return;
            }    
        }

		bestMove = bestIterationMove;
        bestValue = bestIterationValue;
        lastDepth = d;

    }
    isSearching.store(false);
    std::cout << "bestmove " << bestMove << '\n';
    std::cout << "info score cp " << bestValue << "\n";
}
