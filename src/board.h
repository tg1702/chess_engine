#pragma once

#include <iostream>
#include "pieces.h"
#include "types.h"
#include "movegen.h"
#include <string>
#include <cctype>
#include <algorithm>
#include <vector>
#include <memory>

#define PIECE_TYPES 6

class Board{
	private:
		PieceManager pieces;	
		MoveGen generator;	
		MoveList* move_list;

		bool turn = WHITE;

		int actualMoveCount;
		Move actualMoves[MAX_MOVES];
		
		bool castlingRights[4][MAX_MOVES]; 
		int legalMovesCount = 0;	
		//std::vector<Move> legalMoves;

		bool canWhiteKSCastle;
		bool canWhiteQSCastle;
		bool canBlackKSCastle;
		bool canBlackQSCastle;
	
		int enPassantSquare = -1;

	public:
		Board(void);
		Board(std::string);
		
		//~Board() { delete move_list; }
		void makeMove(Move&);	
		void makeMoveHelper(Move&);
		void unmakeMove();
		void unmakeMoveHelper();
		void makeMove(std::string);
		void printBoard(void);
		void addMoveToHistory(Move&);
		bool movePawnFifthRank(Square, Square);
	       	bool movePawnFourthRank(Square, Square);
		
		void setFEN(std::string);
		void whiteKingSideCastle(void);
		void whiteQueenSideCastle(void);
		void blackKingSideCastle(void);
		void blackQueenSideCastle(void);
		void enPassantWhite(Square, Square);
		void enPassantBlack(Square, Square);
		bool isInCheck(bool);
		void generateMoves(void);
		std::vector<Move> generateLegalMoves(void);
		std::vector<Move> generatePseudoLegalMoves(void);
		std::vector<std::string> split_fen(std::string&);
		void parsePieceFen(std::string&);
		
		void parseTurn(std::string&);
		void parseEnPassantSquares(std::string&);
		void parseCastlingRights(std::string&);
		void parseHalfMoveClock(std::string&);
		void parseFullMoveClock(std::string&);
		
		int getActualMoveCount(void);
		void printHistory(void);
		int getMaterialCount(bool);
		bool isGameOver(void);

		inline bool getTurn() { return turn;}
		bool isCheckmated(bool);
		bool isStalemate();	



};
