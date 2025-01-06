#include <iostream>
#include <array>
#include <chrono>


#include <sstream>
#include <cassert>

#include "board.h"
#include "move.h"
#include "perft.h"


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
			p_divide(depth, board);	
			const auto end{std::chrono::steady_clock::now()};
			const std::chrono::duration<double> elapsed_seconds{end - start};
			std::cout << "Perft completed with " << node_count << " total nodes" << '\n';
			std::cout << "Nodes per second= " << (node_count / elapsed_seconds.count()) << '\n';
			std::cout << "Total time : " << elapsed_seconds.count() << "s" << '\n' << '\n';
		}
	
		
	}
	
	return 0;
}

