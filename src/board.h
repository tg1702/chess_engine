#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include "pieces.h"
#include "types.h"
#include "./movegen/movegen.h"
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

		Side turn = Side::WHITE;
		int count = 0;

		int actualMoveCount;
		Move actualMoves[MAX_MOVES * 5];
		
		bool castlingRights[4][MAX_MOVES * 5]; 
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
		
	//	~Board() { delete move_list; }
		//Board(const Board& b) = default;
		//Board& operator=(Board other) {}	
		void unmakeMove();
		void unmakeMoveHelper();
		void makeMoveHelper(const Move&);
		void makeMove(const Move&);
		void makeMove(const std::string&);
		void printBoard(void);
		void addMoveToHistory(const Move&);
		
		bool movePawnFifthRank(const Square, const Square);
	    bool movePawnFourthRank(const Square, const Square);
		
		void setFEN(const std::string);
		void whiteKingSideCastle(void);
		void whiteQueenSideCastle(void);
		void blackKingSideCastle(void);
		void blackQueenSideCastle(void);
		void enPassantWhite(const Square, const Square);
		void enPassantBlack(const Square, const Square);
		bool isInCheck(Side);
		void generateMoves(const MoveType=MoveType::ALL_TYPES);
		std::vector<Move> generateLegalMoves(MoveType=MoveType::ALL_TYPES);
		std::vector<Move> generatePseudoLegalMoves(MoveType=MoveType::ALL_TYPES);
		std::vector<std::string> split_fen(const std::string&);
		void parsePieceFen(const std::string&);
		
		void parseTurn(const std::string&);
		void parseEnPassantSquares(const std::string&);
		void parseCastlingRights(const std::string&);
		void parseHalfMoveClock(const std::string&);
		void parseFullMoveClock(const std::string&);
		
		int getActualMoveCount(void);
		void printHistory(void);
		int getMaterialCount(Side) const;
		bool isGameOver(void);

		inline Side getTurn() { return turn;}
		bool isCheckmated(Side);
		bool isStalemate();	
		bool isInsufficientMaterial();
		bool isDraw();

		Bitboard getBitboard(Side, PieceType) const;


		inline Move getLastMove() {
			if (actualMoveCount > 0) {
				return actualMoves[actualMoveCount - 1];
			}
			throw std::out_of_range("No moves have been made yet.");
		}

};

#endif