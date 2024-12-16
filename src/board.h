#include <iostream>
#include "pieces.h"
#include "types.h"
#include "move.h"
#include <cctype>
#include <algorithm>
#include <vector>
#include <stack>

#define PIECE_TYPES 6

class Board{
	private:
		PieceManager pieces = PieceManager();
		std::vector<Move> generatedMoves;
		int moveCount;
		int captures=0;	
		std::stack<Move> actualMoves;
		bool turn = WHITE;
		bool hasMovedWhiteKing;
		bool hasMovedA1Rook;
		bool hasMovedH1Rook;
		bool hasMovedBlackKing;
		bool hasMovedA8Rook;
		bool hasMovedH8Rook;
		
		int enPassantWhiteLeftFromSquare;
		int enPassantBlackLeftFromSquare;
		int enPassantWhiteRightFromSquare;
		int enPassantBlackRightFromSquare;
		
		int enPassantWhiteToSquare;
		int enPassantBlackToSquare;
		bool enPassantWhiteFlag;
		bool enPassantBlackFlag;
	public:
		Board(void);
		Board(PieceArgs args);

		void addEncodedMove(void);
		void getGeneratedEncodedMoves(void);
		void generateEncodedMoves(void);	
		std::string makeMove(int, int, int=NORMAL);	
		std::string unmakeMove();
		void printBoard(void);
		void addCastlingRights(void);
		void addMoveToHistory(Move);
		bool movePawnFifthRank(int, int);
	       	bool movePawnFourthRank(int, int);
		bool isValidMove(int, int, int);	
		void addEnPassantRights(void);
		void removeEnPassantRights(void);

		bool isCastlingMove(int, int);
		bool isWhiteKSCastlingMove(int, int);
		bool isWhiteQSCastlingMove(int, int);
		bool isBlackKSCastlingMove(int, int );
		bool isBlackQSCastlingMove(int, int);
		void whiteKingSideCastle(void);
		void whiteQueenSideCastle(void);
		void blackKingSideCastle(void);
		void blackQueenSideCastle(void);
		void promotePawns(bool, int, int, int);
		void enPassantWhite(int, int);
		void enPassantBlack(int, int);
		bool isWhiteEnPassantMove(int);
		bool isBlackEnPassantMove(int);
		bool isInCheck(bool);
		int updateToCaptureFlag(int);
		std::vector<Move> generateMoves(void);
		int getActualMoveCount(void);
		void setActualMoveCount(int);
		void printHistory(void);
inline void setMoves(){
	generatedMoves = pieces.getEncodedMove();
}

inline std::vector<Move> getMoves() {
	return generatedMoves;
}

inline int getMoveCount(){ return generatedMoves.size(); }

inline bool getTurn() { return turn;}
};
