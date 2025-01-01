#include <iostream>
#include <array>
#include <chrono>
#include <sstream>

#include "../src/board.h"
#include "../src/move.h"
#include "../src/types.h"

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
//		board.printBoard();
		return 1ULL;
	}



        move_list = board.generatePseudoLegalMoves();
	
	bool turn = board.getTurn();

        for(auto& move: move_list){
                board.makeMove(move);

                if (!board.isInCheck(turn))
			level_count += p_divide(depth - 1, board);	

		
		if (depth == DEPTH){
			std::cout << pieceSquareNames[move.getFrom()] << pieceSquareNames[move.getTo()] << ": " << level_count << '\n';
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
		 board.makeMove(move);                                                                                                                               level_count += divide(depth - 1, board);
	 	 board.unmakeMove();

		 if (depth == DEPTH){
			std::cout << pieceSquareNames[move.getFrom()] << pieceSquareNames[move.getTo()] << ": " << level_count << '\n';	
			level_count = 0;
		 }
	 }	
	
	return level_count;
}
int main(){	//PieceArgs p{};
	std::string word;
	std::string w;
	char split_char = ' ';


	//
	std::array<std::string, 5> fens = {
		"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
		"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 5 9",
		"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3KR2 b kq - 1 1", // kiwipete debug depth 3
		//"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"		
	
	};

	std::vector<std::string> words;
	
	std::string s;	

	Board board = Board();

	while ( std::getline(std::cin, s) ){
		if (s == "quit")
			break;
		words.clear();
	
		std::stringstream ss(s);	
		
		while ( std::getline(ss, w, split_char)) {
			words.push_back(w);
		}	
		
		if ((words.size() == 8) && words[0] == "position" && words[1] == "fen"){
			std::string fen = words[2] + " " + words[3] + " " + words[4] + " " + words[5] + " " + words[6] + " " + words[7];

			board.setFEN(fen);
			board.printBoard();

		}

		if ((words.size() == 3) && words[0] == "go" && words[1] == "perft"){
			const int depth = words[2][0] - '0';
			DEPTH = depth;
			node_count = 0ULL;
			
			const auto start{std::chrono::steady_clock::now()};
			const auto perft_result{p_divide(depth, board)};	
			const auto end{std::chrono::steady_clock::now()};
			const std::chrono::duration<double> elapsed_seconds{end - start};
			std::cout << "Perft completed with " << node_count << " total nodes" << '\n';
			std::cout << "Nodes per second= " << (node_count / elapsed_seconds.count()) << '\n';
			std::cout << "Total time : " << elapsed_seconds.count() << "s" << '\n' << '\n';
		}
	
		
	}	
	return 0;
}
