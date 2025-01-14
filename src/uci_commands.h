#include <iostream>
#include <string>

class Executor{
	public:
		void printResponse(void);
		void performSearch(void);
		void setBoard(Board&);

};

class Command{
	virtual void execute(void) = 0;
};

class PositionCommand : public Command{
	private:
		std::vector<std::string> params;
	
	public:
		PositionCommand(Board& board, std::vector<std::string> p): params(p) {} 
		void execute(void) override;
};

class GoCommand : public Command{
	private:
		std::vector<std::string> params;

	public:
		GoCommand(Board& board, std::vector<std::string> p): params(p) {}
		void execute(void) override;
};

class UCICommand : public Command{
	public:
		void execute(void) override;
};

class ReadyCommand: public Command{
	public:
		void execute(void) override;
};
