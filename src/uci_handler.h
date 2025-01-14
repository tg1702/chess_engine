#include <iostream>
#include <vector>
#include <string>
#include "uci_commands.h"
#include "utils.h"

class UCICommandHandler{
	public:
		UCICommandHandler() = default;
		~UCICommandHandler() {delete command;}
		void parse(std::string, Board&);
	private:
		Command* command;
};


void UCICommandHandler::parse(std::string input, Board& board){
	std::vector<std::string> parsedInput = utils::split_string(input);

	if (parsedInput[0] == "go"){
		command = new GoCommand(board, parsedInput);
	}
	
	if (parsedInput[0] == "position"){
		command = new PositionCommand(board, parsedInput);
	}

	if (parsedInput[0] == "uciready"){
		command = new ReadyCommand();
	}

	if (parsedInput[0] == "uci"){
		command = new UCICommand();
	}	

}
