#include <iostream>
#include "pieces.h"
#include "types.h"
#include "movegen.h"
#include <cctype>
#include <algorithm>
#include <vector>

#define PIECE_TYPES 6

class Board{
	private:
		PieceManager pieces;	
		MoveGen generator;	
		MoveList* move_list = new MoveList();

		bool turn = WHITE;
		


		int actualMoveCount = 0;
		Move actualMoves[10];
		
		bool whiteKSKRMoved;
		bool blackKSKRMoved;
		bool whiteQSKRMoved;
		bool blackQSKRMoved;	
		
		bool canWhiteKSCastle;
		bool canWhiteQSCastle;
		bool canBlackKSCastle;
		bool canBlackQSCastle;

		bool hasMovedWhiteKing;
		bool hasMovedA1Rook;
		bool hasMovedH1Rook;
		bool hasMovedBlackKing;
		bool hasMovedA8Rook;
		bool hasMovedH8Rook;
	
		int enPassantSquare = -1;

	public:
		Board(void);
		Board(PieceArgs&, bool, bool, bool, bool, bool, int, int, int);
		Board(std::string);
		
		~Board() { delete move_list; }
		void getGeneratedEncodedMoves(void);
		void generateEncodedMoves(void);	
		void makeMove(Move&);	
		void makeMoveHelper(Move&);
		void unmakeMove();
		void unmakeMoveHelper();
		void printBoard(void);
		void addCastlingRights(void);
		void addMoveToHistory(Move&);
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
		bool isInCheck(bool);
		int updateToCaptureFlag(int);
		void generateMoves(void);
		std::vector<Move> generateLegalMoves(void);
		std::vector<Move> generatePseudoLegalMoves(void);
		std::vector<string> split_fen(std::string&);
		void parsePieceFen(std::string&);
		
		void parseTurn(std::string&);
		void parseEnPassantSquares(std::string&);
		void parseCastlingRights(std::string&);
		void parseHalfMoveClock(std::string&);
		void parseFullMoveClock(std::string&);
		
		int getActualMoveCount(void);
		void setActualMoveCount(int);
		void printHistory(void);

//inline int getLegalMoveCount() { return legalMoves.getMoveCount(); }
		inline bool getTurn() { return turn;}




};
