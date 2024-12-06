#include <iostream>
#include "pieces.h"
#include "types.h"
#include <cctype>
#include <algorithm>

#define PIECE_TYPES 6

class Board{
	private:
		PieceManager pieces = PieceManager();
		std::array<unsigned int, 256> generatedMoves;
		int actualMoves[256];
		int moveCount;
		bool turn;
		bool hasMovedWhiteKing;
		bool hasMovedA1Rook;
		bool hasMovedH1Rook;
		bool hasMovedBlackKing;
		bool hasMovedA8Rook;
		bool hasMovedH8Rook;
		bool enPassantSquareWhite;
		bool enPassantSquareBlack;
	public:
		Board(void);
		Board(PieceArgs args);

		std::array<unsigned int, 256> getMove(void); 
		int getMoveCount(void);
		
		void addEncodedMove();
		void getGeneratedEncodedMoves();
		void generateEncodedMoves();	
		std::string makeMove(int, int, bool, int=0);	
		void unmakeMove();
		void printBoard(void);
		void addCastlingRights(void);
};

inline std::array<unsigned int, 256> Board::getMove() {
	return pieces.getEncodedMove();
}

inline int Board::getMoveCount(){ return 256 - std::count(generatedMoves.begin(),generatedMoves.end(), 0); }
