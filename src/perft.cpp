#include <iostream>
#include <array>
#include <chrono>

#include "../src/board.h"
#include "../src/move.h"
#include "../src/types.h"

#define DEPTH 6

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
			std::cout << pieceSquareNames[move.getFrom()] << pieceSquareNames[move.getTo()] << ": " << level_count << endl;	
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
		 board.makeMove(move);                                                                                                                               nodes += perft(depth - 1, board);
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
		 board.makeMove(move);                                                                                                                               level_count += divide(depth - 1, board);
	 	 board.unmakeMove();

		 if (depth == DEPTH){
			std::cout << pieceSquareNames[move.getFrom()] << pieceSquareNames[move.getTo()] << ": " << level_count << endl;	
			level_count = 0;
		 }
	 }	
	
	return level_count;
}
int main(){
	Board board = Board();
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
	
	const auto start{std::chrono::steady_clock::now()};
	const auto perft_result{perft(DEPTH, board)};	
	const auto end{std::chrono::steady_clock::now()};
	const std::chrono::duration<double> elapsed_seconds{end - start};
	
	std::cout << "Perft completed with " << perft_result << " total nodes" << '\n';
	std::cout << "Nodes per second= " << (perft_result / elapsed_seconds.count()) << '\n';
	std::cout << "Total time : " << elapsed_seconds.count() << "s" << '\n';
	return 0;
}
