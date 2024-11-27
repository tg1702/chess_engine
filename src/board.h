#include <iostream>
#include "pieces.h"
#include "types.h"
#define PIECE_TYPES 7

class Board{
	private:
		PieceManager pieces;
		int generatedMoves[256];
		int actualMoves[256];
		int moveCount;
		bool turn;
		bool hasMovedWhiteKing;
		bool hasMovedA1Rook;
		bool hasMovedH1Rook;
		bool hasMovedBlackKing;
		bool hasMovedA8Rook;
		bool hasMovedH8Rook;
	public:
		Board(){
			pieces = PieceManager();
			moveCount = 0;
			turn = false;
			hasMovedWhiteKing = false;
			hasMovedA1Rook = false;
			hasMovedH1Rook = false;
			hasMovedBlackKing = false;
			hasMovedA8Rook = false;
			hasMovedH8Rook = false;
				
		}
		int getMove() {return pieces.getEncodedMove();}
		void addEncodedMove();
		void getGeneratedEncodedMoves();
		void makeMove(int, int, bool);	
		void unmakeMove();
};

void Board::makeMove(int from, int to, bool side){
	bool validCapture = false;
	for (int i = 0; i < PIECE_TYPES; i++){
		if ( i == KING){
			if (from == E1 && to == G1 && pieces.canKingSideCastle(WHITE) && !hasMovedA1Rook && !hasMovedWhiteKing){
				pieces.setAnyPosBB( WHITE, KING, pieces.getPiecesBB(WHITE, KING) ^ bitset(from));	
				pieces.setAnyPosBB( WHITE, KING, pieces.getPiecesBB(WHITE, KING) ^ bitset(from));
				
				
				pieces.setAnyPosBB( WHITE, ROOK, pieces.getPiecesBB(WHITE, ROOK) ^ bitset(H1));
				pieces.setAnyPosBB( WHITE, ROOK,pieces.getPiecesBB(WHITE, ROOK) ^ bitset(F1));
			}
			else if (from == E1 && to == C1 && pieces.canQueenSideCastle(side)){
					
				pieces.setAnyPosBB( WHITE, KING ,pieces.getPiecesBB(WHITE, KING) ^ bitset(from));
				pieces.setAnyPosBB( WHITE, KING ,pieces.getPiecesBB(WHITE, KING) ^ bitset(to));
				
				pieces.setAnyPosBB( WHITE, ROOK ,pieces.getPiecesBB(WHITE, ROOK) ^ bitset(A1));
				pieces.setAnyPosBB( WHITE, ROOK ,pieces.getPiecesBB(WHITE, ROOK) ^ bitset(C1));
				
			}
		}

		if ((pieces.getMovesBB(side, i) & bitset(from)) || (pieces.getMovesBB(side, i) & bitset(to))){
			
			pieces.setAnyPosBB( side, i ,pieces.getPiecesBB(side, i) ^ bitset(from));
			pieces.setAnyPosBB( side, i ,pieces.getPiecesBB(side, i) ^ bitset(to));
			
			validCapture = true;

			if (i == KING && side == WHITE) hasMovedWhiteKing = true;
			if (i == KING && side == BLACK) hasMovedBlackKing = true;
			if (i == ROOK && (pieces.getPiecesBB(side, ROOK) & bitset(A1))	&& side == WHITE) hasMovedA1Rook = true;
			if (i == ROOK && (pieces.getPiecesBB(side, ROOK) & bitset(H1))	&& side == WHITE) hasMovedH1Rook = true;
			if (i == ROOK && (pieces.getPiecesBB(side, ROOK) & bitset(A8))	&& side == BLACK) hasMovedA8Rook = true;
			if (i == ROOK && (pieces.getPiecesBB(side, ROOK) & bitset(H8))	&& side == BLACK) hasMovedH8Rook = true;
		}


		if (validCapture && (pieces.getMovesBB(side, i) & bitset(from)))
			pieces.setAnyPosBB( side, i ,pieces.getPiecesBB(side, i) ^ bitset(to));

	}

	if (!hasMovedWhiteKing && !hasMovedA1Rook && pieces.canKingSideCastle(WHITE)){
		pieces.addKingSideCastlingRights(WHITE);	
	}
		
}
