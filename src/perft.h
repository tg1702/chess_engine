#ifndef PERFT_H
#define PERFT_H

#include <iostream>
#include <array>
#include <chrono>


#include <sstream>
#include <cassert>

#include "board.h"
#include "move.h"
#include "types.h"
//#include "magic.h"

int DEPTH = 0;
static uint64_t node_count = 0ULL;	

uint64_t p_perft(int depth, Board& board){
	
	uint64_t nodes = 0ULL;

	std::vector<Move> move_list;
	
	
	if (depth == 0)
	{
		return 1ULL;
	}



        move_list = board.generatePseudoLegalMoves();
	
	bool turn = board.getTurn();

        for(auto& move: move_list){
                board.makeMove(move);

                if (!board.isInCheck(turn))
			nodes += p_perft(depth - 1, board);	

		
		board.unmakeMove();	
	}

		
	return nodes;
}
uint64_t p_divide(int depth, Board& board){
	uint64_t level_count = 0ULL;

	
	std::vector<Move> move_list;                                                                                                               
	
	if (depth == 0)
	{
		return 1ULL;
	}


        move_list = board.generatePseudoLegalMoves();
	
	bool turn = board.getTurn();

        for(auto& move: move_list){
                board.makeMove(move);

                if (!board.isInCheck(turn))
			level_count += p_divide(depth - 1, board);	

		
		if (depth == DEPTH){
			std::cout << move << ": " << level_count << '\n';
			node_count += level_count;
			level_count = 0;
		}

		
		board.unmakeMove();	
	}

	return level_count;
}


uint64_t perft(int depth, Board& board){
	
	uint64_t nodes = 0ULL;

	 std::vector<Move> move_list;                                                                                                               
	 move_list = board.generateLegalMoves();                                                                                
	 
	 if (depth == 1)
         {
                return move_list.size();
         }                                                                                                                                                   
	 
	 for(auto& move: move_list){                                                                                                     
		 board.makeMove(move);                                                                                                   nodes += perft(depth - 1, board);
	 	 board.unmakeMove();

	 }	
	
	return nodes;
}


uint64_t divide(int depth, Board& board){
	
	uint64_t level_count = 0ULL;
	
	std::vector<Move> move_list;
	move_list = board.generateLegalMoves();                                                                                
	 
	 if (depth == 1)
         {
                return move_list.size();
         }                                                                                                                                                   
	 
	 for(auto& move: move_list){                                                                                                     
		 board.makeMove(move);                                                                                                   level_count += divide(depth - 1, board);
	 	 board.unmakeMove();

		 if (depth == DEPTH){
			std::cout << move << ": " << level_count << '\n';	
			level_count = 0;
		 }
	 }	
	
	return level_count;
}

#endif