#include <iostream>
#include <array>
#include "move.h"
#include "types.h"

#pragma once
class MoveList{
	private:
		void addPossibleMove(int, uint64_t, bool, piece_bb_types);
		std::array<Move, 256> moves;
		int move_count = 0;
	public:
		void generateMoves(bool, const PieceBB*, uint64_t);
		void generateMoves(void);
		void addKingSideCastling(bool);
		void addQueenSideCastling(bool);
		void generateKnightMoves(bool, uint64_t, uint64_t);
		void generatePawnMoves(bool, uint64_t, uint64_t, uint64_t);
		void generateKingMoves(bool, uint64_t, uint64_t);
		void generateRookMoves(bool, uint64_t, uint64_t, uint64_t);
		void generateBishopMoves(bool, uint64_t, uint64_t, uint64_t);
		void generateQueenMoves(bool, uint64_t, uint64_t, uint64_t);
		bool isPromoting(bool, int, int);
		inline void clearMoves(void){move_count = 0;}
		inline std::array<Move, 256> getMoves() {return moves;}				
		inline void addEnPassantRights(bool side, int from, int to){moves[move_count] = Move(EN_PASSANT_FLAG, from, to, PAWN, PAWN); move_count++;}
		inline void removeEnPassantRights(bool side, int from, int to){move_count--;}
		inline int getMoveCount(void) {return move_count;}
};
