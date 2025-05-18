#include <iostream>
#include <array>
#include <chrono>
#include <climits>
#include <thread>
#include <atomic>
#include <cmath>
#include <mutex>
#include <random>
#include <algorithm>
#include <random>

#include "../board.h"
#include "../movegen/move.h"
#include "search.h"
#include "../uci/timer.h"
#include "pst.h"
#include "../types.h"
#include "move_ordering.h"

std::atomic<bool> isSearching{false};
std::mt19937 mt{std::random_device{}()};
std::uniform_real_distribution<double> distribution(0.0,1.0);
int nodes = 0;

float evaluate(Board& board){
    int w_mg_score = 0;
    int b_mg_score = 0;
    int w_eg_score = 0;
    int b_eg_score = 0;
    int gamePhase = 0;

    for (const Square& sq: Squares){
        for (const PieceType& pc: PieceTypes){
            if (board.getBitboard(WHITE, pc) & utils::to_bb(sq)){

                w_mg_score += MaterialValues[pc] + mg_pesto_table[pc][sq];
                w_eg_score += MaterialValues[pc] + eg_pesto_table[pc][sq];
                gamePhase += gamePhaseInc[pc];
            }
                
            
            else if (board.getBitboard(BLACK, pc) & utils::to_bb(sq)){

                b_mg_score += MaterialValues[pc] + mg_pesto_table[pc][56 ^ sq];
                b_eg_score += MaterialValues[pc] + eg_pesto_table[pc][56 ^ sq];
                gamePhase += gamePhaseInc[pc];
            }
                

        }
    }
    ++nodes;

    int mgScore = w_mg_score - b_mg_score;
    int egScore = w_eg_score - b_eg_score;

    int mgPhase = gamePhase;
    if (mgPhase > 24) mgPhase = 24; 
    int egPhase = 24 - mgPhase;
    return (mgScore * mgPhase + egScore * egPhase) / 24;
}

float negamax(int depth, Board& board, float alpha, float beta, int colour){
    Moves allMoves = board.generateLegalMoves();

    Side turn = (colour == 1) ? WHITE:BLACK;
    if (board.isCheckmated(turn)) return -INT_MAX + depth;

    if (board.isDraw()) return 0;    

    if (depth == 0){

        Moves captureSet;

         auto captures = std::remove_if(captureSet.begin(),
        captureSet.end(),
        [](const Move& move){
            return (
            move.getFlag() == Flag::CAPTURE || 
            move.getFlag() ==Flag::QUEEN_PROMOTION_CAPTURE || 
            move.getFlag() ==Flag::ROOK_PROMOTION_CAPTURE || 
            move.getFlag() ==Flag::BISHOP_PROMOTION_CAPTURE || 
            move.getFlag() == Flag::KNIGHT_PROMOTION_CAPTURE ||
            move.getFlag() == Flag::EN_PASSANT
            );
        });

        captureSet.erase(captures, captureSet.end());

        if (captureSet.size() == 0)
            return colour * evaluate(board);
        else
            return quiescence_search(board, 4, colour, alpha, beta);
        

    }
        

    
    float value = -INFINITY;
    
    
    score_moves(allMoves);

    for(size_t i = 0; i < allMoves.size(); ++i){

        pick_move(allMoves, i);

            
        Move move = allMoves[i];
        
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
    Move bestMove{Move(Flag::NORMAL, H1, H1, KING)};
	float bestValue = -INFINITY;
    int lastDepth = 0;

    nodes = 0;

	Moves allMoves = board.generateLegalMoves();  

    score_moves(allMoves);

    for (int d = 1; d <= depth && isSearching; d++){  // Start from depth 1
        float alpha = -INFINITY;
        float beta = INFINITY;
		float bestIterationValue = -INFINITY;
		Move bestIterationMove = allMoves[0];
        
        if (bestValue == INT_MAX)
        {
            
            break;
        }

        for (size_t i = 0; i < allMoves.size(); ++i){
            pick_move(allMoves, i);

            
            const Move move = allMoves[i];

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
                std::cout << "info score cp " << bestValue << " depth " << lastDepth << " nodes " << nodes << "\n";
                isSearching.store(false);
                return;
            }    
        }

        
		bestMove = bestIterationMove;
        bestValue = bestIterationValue;
        lastDepth = d;

    }
    isSearching.store(false);
    timer.stop();

    std::cout << "bestmove " << bestMove << '\n';
    std::cout << "info score cp " << bestValue << " depth " << lastDepth << " nodes " << nodes << "\n";

}


float quiescence_search(Board& board, int depth, int colour, float alpha, float beta){

    Moves captures = board.generateLegalMoves(MoveType::CAPTURES);

    Side turn = (colour == 1) ? WHITE:BLACK;
    if (board.isCheckmated(turn)) return -INT_MAX + depth;
    if (board.isDraw()) return 0;    


    
    float stand_pat = colour * evaluate(board);

    if( stand_pat >= beta)
        return beta;
    if( alpha < stand_pat )
        alpha = stand_pat;

    if (depth == 0)
        return stand_pat;

   
    score_moves(captures);
  
        
    float value = stand_pat;

    for (size_t i = 0; i < captures.size(); i++){
            pick_move(captures, i);
            
            const Move move = captures[i];

            board.makeMove(move);
            value = std::max(value, -quiescence_search(board, depth - 1, -colour, -beta, -alpha));
            board.unmakeMove();

            alpha = std::max(alpha, value);
            
            if (alpha >= beta){
                break; 
            } 
        
        
    }
    
    return value;
}

void score_moves(Moves& moves){

    for (Move& move: moves){
        move.set_score(MVV_LVA[move.getToPiece()][move.getFromPiece()]);
    }
}

void pick_move(Moves& moves, size_t start_index){
    for (size_t i = start_index; i < moves.size(); i++){
        
        if (moves[i].get_score() > moves[start_index].get_score()){
            std::swap(moves[i], moves[start_index]);
        }
    }

}
