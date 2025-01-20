#include <iostream>
#include <array>
#include "types.h"
#include <memory>
#pragma once
class MoveGen{
	private:
		BoardState state;
		void addPossibleMove(Square, uint64_t, PieceType, MoveList*);
	public:
		void setState(BoardState&);
		void generateMoves(MoveList*);
		void generateKnightMoves(MoveList*);
		void generateWhitePawnMoves(MoveList*);
		void generateBlackPawnMoves(MoveList*);
		void generateKingMoves(MoveList*);
		void generateRookMoves(MoveList*);
		void generateBishopMoves(MoveList*);
		void generateQueenMoves(MoveList*);

		

		
};
