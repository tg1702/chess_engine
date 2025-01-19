
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
#include "perft.h"
#include "uci_handler.h"
#include "search.h"
#include "timer.h"


int main(){
	std::string word;

	Board board = Board();

	std::thread s_thread;
	std::string s;
	char split_char = ' ';

	int depth = 10;
	
	std::vector<std::string> words;

	while ( std::getline(std::cin, s) ){
	
		std::stringstream ss(s);	
		
		std::string w;
		
			
		words.clear();
		while ( std::getline(ss, w, split_char)) {
			words.push_back(w);
		}

		
		if (words[0] == "quit")
			break;	
	

		if (words[0] == "position" && words[1] == "fen"){
			if (s_thread.joinable()){
					isSearching = false;
					s_thread.join();
				}

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
			if (s_thread.joinable()){
					isSearching = false;
					s_thread.join();
				}
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
			
			int wtime = utils::findPos<std::string>(words, "wtime");
			int btime = utils::findPos<std::string>(words, "btime");
			int winc = utils::findPos<std::string>(words, "winc");
			int binc = utils::findPos<std::string>(words, "binc");
			int dp = utils::findPos<std::string>(words, "depth");	
			if (wtime != -1 && btime != -1 && words.size() > 1){
				const int wtimeVal = std::stol(words[wtime+1]);
				const int btimeVal = std::stol(words[btime+1]);

				(turn == -1) ?	defaultTime = btimeVal/25 : defaultTime = wtimeVal/25;

			}
		

				
			if (dp != -1 && words.size() > 1){
				depth = words[dp+1][0] - '0';
			}
		
			if (winc != -1 && binc != -1 && words.size() > 1){
 				const int wincVal = std::stol(words[winc+1]);
                                const int bincVal = std::stol(words[binc+1]);
				(turn == -1) ? defaultTime += wincVal/25 : defaultTime += bincVal/25; 
			}

			if (words[1] == "perft" && (words.size() == 3)){
				DEPTH = words[2][0] - '0';
				node_count = 0ULL;
			
				Timer t = Timer();

				t.start();
				p_divide(DEPTH, board);	
				t.stop();

				std::cout << "Perft completed with " << node_count << " total nodes" << '\n';
				std::cout << "Nodes per second= " << (node_count / t.elapsedTime()) << '\n';
				std::cout << "Total time : " << t.elapsedTime() << "s" << '\n' << '\n';
			}

			else{
				//if (!stopSearching) continue;
				
				if (!isSearching && s_thread.joinable()){
					s_thread.join();
				}
				if (!s_thread.joinable()){
					
					s_thread = std::thread(search, std::ref(board), turn, defaultTime, depth);
					isSearching = true;
				}



			}
		}


				
		if ((words[0] == "stop" && s_thread.joinable())){
			isSearching.store(false);
		}

		
		}




				if (s_thread.joinable()){
					s_thread.join();
				}
	return 0;
}

