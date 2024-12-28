#include <iostream>
#include <array>
#include "types.h"

#pragma once
class MoveGen{
	private:
		void addPossibleMove(int, uint64_t, bool, int, MoveList*);
	public:
		void generateMoves(bool, const PieceBB*, uint64_t, MoveList*, int, bool, bool, bool, bool);
		void addKingSideCastling(bool, MoveList*);
		void addQueenSideCastling(bool, MoveList*);
		void generateKnightMoves(bool, uint64_t, uint64_t, MoveList*);
		void generateWhitePawnMoves(bool, uint64_t, uint64_t, uint64_t, MoveList*, int);
		void generateBlackPawnMoves(bool, uint64_t, uint64_t, uint64_t, MoveList*, int);
		void generateKingMoves(bool, uint64_t, uint64_t, MoveList*, bool, bool, bool, bool);
		void generateRookMoves(bool, uint64_t, uint64_t, uint64_t, MoveList*);
		void generateBishopMoves(bool, uint64_t, uint64_t, uint64_t, MoveList*);
		void generateQueenMoves(bool, uint64_t, uint64_t, uint64_t, MoveList*);
		bool isPromoting(bool, int, int);

		

		
};
