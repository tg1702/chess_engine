#include <iostream>
#include <array>
#include <chrono>

#include "../src/board.h"
#include "../src/move.h"
#include "../src/types.h"

#define DEPTH 7

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
	//PieceArgs p{};
	//
	std::array<std::string, 3> fens = {
		"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
		"rnbqkbnr/ppp3pp/8/3pppP1/8/5N2/PPPPPP1P/RNBQKB1R w KQkq e6 0 4",
		"rnbqkbnr/ppp2ppp/8/3pp1P1/8/5N2/PPPPPP1P/RNBQKB1R b KQkq - 1 3"
	};
	Board board = Board(fens[0]);
	board.printBoard();


	board.printBoard();

	const auto start{std::chrono::steady_clock::now()};
	const auto perft_result{p_divide(DEPTH, board)};	
	const auto end{std::chrono::steady_clock::now()};
	const std::chrono::duration<double> elapsed_seconds{end - start};
	std::cout << "Perft completed with " << perft_result << " total nodes" << '\n';
	std::cout << "Nodes per second= " << (perft_result / elapsed_seconds.count()) << '\n';
	std::cout << "Total time : " << elapsed_seconds.count() << "s" << '\n';
	

	
	
	return 0;
}
