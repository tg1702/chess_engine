#ifndef MOVEGEN_H
#define MOVEGEN_H

#include <iostream>
#include <cstdint>
#include <memory>

#include "magic.h"
#include "move.h"
#include "pieces.h"
#include "types.h"
#include "utils.h"



class MoveGen{
	private:
		BoardState state;
		void addPossibleMove(Square, uint64_t, PieceType, MoveList*);
	public:
		void setState(BoardState&);
		
		void generateMoves(MoveList*);
		
		template <MoveType Type>
		void generateKnightMoves(MoveList*);
		
		template <MoveType Type>
		void generateWhitePawnMoves(MoveList*);
		
		template <MoveType Type>
		void generateBlackPawnMoves(MoveList*);
		
		template <MoveType Type>
		void generateKingMoves(MoveList*);
		
		template <MoveType Type>
		void generateRookMoves(MoveList*);
		
		template <MoveType Type>
		void generateBishopMoves(MoveList*);
		
		template <MoveType Type>
		void generateQueenMoves(MoveList*);

		

		
};




#endif