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
		int moveCount;
		
		int actualMoves[256];
		int actualMoveCount=0;	
		bool turn;
		bool hasMovedWhiteKing;
		bool hasMovedA1Rook;
		bool hasMovedH1Rook;
		bool hasMovedBlackKing;
		bool hasMovedA8Rook;
		bool hasMovedH8Rook;
		int enPassantWhiteSquare;
		int enPassantBlackSquare;
		bool enPassantWhiteFlag;
		bool enPassantBlackFlag;
	public:
		Board(void);
		Board(PieceArgs args);

		std::array<unsigned int, 256> getMove(void); 
		int getMoveCount(void);
	 	void setMoves(void);	
		void addEncodedMove(void);
		void getGeneratedEncodedMoves(void);
		void generateEncodedMoves(void);	
		std::string makeMove(int, int, bool, int=0);	
		void unmakeMove();
		void printBoard(void);
		void addCastlingRights(void);
		void addMoveToHistory(int);
		bool movePawnFifthRank(int, int);
	       	bool movePawnFourthRank(int, int);	
};


inline void Board::setMoves(){
	generatedMoves = pieces.getEncodedMove();
}
inline std::array<unsigned int, 256> Board::getMove() {
	return generatedMoves;
}

inline int Board::getMoveCount(){ return 256 - std::count(generatedMoves.begin(),generatedMoves.end(), 0); }
