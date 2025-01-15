#include <iostream>
#include <array>
#include <chrono>
#include <climits>


#include "board.h"
#include "move.h"
#include "perft.h"
#include "uci_handler.h"
#include "search.h"
#include "timer.h"

int main(){
	std::string word;
	std::string w;
	char split_char = ' ';

	Board board = Board();
	
	std::vector<std::string> words;
	
	std::string s;	


	while ( std::getline(std::cin, s) ){
		if (s == "quit")
			break;
		words.clear();
	
		std::stringstream ss(s);	
		
		while ( std::getline(ss, w, split_char)) {
			words.push_back(w);
		}	
		
		if (words[0] == "position" && words[1] == "fen"){
			std::string fen = words[2] + " " + words[3] + " " + words[4] + " " + words[5] + " " + words[6] + " " + words[7];

			board.setFEN(fen);
			
			if (words[8] == "moves"){	
				for(size_t i = 9; i < words.size(); i++){
					board.makeMove(words[i]);	
				}
			}	
			
			board.printBoard();

		}

		if (words[0] == "position" && words[1] == "startpos"){
			board.setFEN(STARTPOS_FEN);
		
			if (words[2] == "moves"){	
				for(size_t i = 3; i < words.size(); i++){
					board.makeMove(words[i]);	
				}
			}
			board.printBoard();
		}


		if (words[0] == "go"){
			
			int turn = board.getTurn() ? -1 : 1;
			int defaultTime = INT_MAX;	
			
			if (words[1] == "wtime" && words[3] == "btime"){
				const int wtimeVal = std::stoi(words[2]);
				const int btimeVal = std::stoi(words[4]);

				(turn == -1) ?	defaultTime = btimeVal/25 : defaultTime = wtimeVal/25;

			}
			
			if (words[1] == "perft" && (words.size() == 3)){
				const int depth = words[2][0] - '0';
				DEPTH = depth;
				node_count = 0ULL;
			
				Timer t = Timer();

				t.start();
				p_divide(depth, board);	
				t.stop();

				std::cout << "Perft completed with " << node_count << " total nodes" << '\n';
				std::cout << "Nodes per second= " << (node_count / t.elapsedTime()) << '\n';
				std::cout << "Total time : " << t.elapsedTime() << "s" << '\n' << '\n';
			}

			else if (words[1] == "depth" && words.size() == 3){
				const int depth = words[2][0] - '0';
				std::cout << "bestmove " << search(board, turn, depth) << '\n'; 
			}
			else {

				std::cout << "bestmove " << search(board, turn, defaultTime) << '\n'; 
			}
		}		
	}
	
	return 0;
}

