
#pragma once

#include <iostream>
#include "types.h"
#include "utils.h"

#define SIDES 2
#define MAX_PIECE_TYPES 7



class PieceManager{
	private:
		uint64_t Pieces[SIDES][MAX_PIECE_TYPES];
	public:


		inline void setAnyPosBB(bool side, PieceType type, uint64_t pos){
			this->Pieces[side][type] = pos;
		}


	inline void setSidePiecesBB(uint64_t all_pieces_bb, bool side){
		this->Pieces[side][ALL] = all_pieces_bb;
		}

	inline void updatePiecesBB(bool side, PieceType type, uint64_t bb){
		this->Pieces[side][type] = bb;
	}

	inline void setSidePiecesBB(bool side){
		this->Pieces[side][ALL] = this->Pieces[side][KING] | this->Pieces[side][QUEEN] | this->Pieces[side][ROOK] | this->Pieces[side][BISHOP] | this->Pieces[side][KNIGHT] | this->Pieces[side][PAWN];
	}


	inline uint64_t getPiecesBB(bool side, PieceType type){
		return this->Pieces[side][type];
	}
	
		
			

	inline void movePiece(bool side, PieceType type, Square from, Square to){
		clearPiece(side, type, from);
		addPiece(side, type, to);
	}

	inline void clearPiece(bool side, PieceType type, Square square){
		uint64_t curBB = getPiecesBB(side, type);
		bitclear(curBB, square);
		setAnyPosBB( side, type , curBB);
	
	}

	inline void addPiece(bool side, PieceType type, Square on){
		setAnyPosBB(side, type, getPiecesBB(side, type) | bitset(on));
	}
		PieceManager(void);
		PieceManager(PieceArgs& args);
		void setBoard(PieceArgs& args);
		
		bool canKingSideCastle(bool);
		bool canQueenSideCastle(bool);		
		bool isAttacked(bool, Square);
};

