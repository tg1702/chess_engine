
#include <iostream>
#include "types.h"
#include "magic.h"
#include "utils.h"
#include "lookup.h"

#define SIDES 2
#define MAX_PIECE_TYPES 7

#pragma once


class PieceManager{
	private:
		uint64_t Pieces[SIDES][MAX_PIECE_TYPES];
		uint64_t attacks[SIDES][MAX_PIECE_TYPES];
		uint64_t all_pieces;
		
		std::array<unsigned int, 256> generatedMoves = {0ULL};
		int move_count;
	public:


		inline void setAnyPosBB(bool side, int type, uint64_t pos){
			this->Pieces[side][type] = pos;
		}

		inline void setAllPiecesBB(uint64_t all_pieces_bb){
			this->all_pieces = all_pieces_bb;
		}

	inline void setSidePiecesBB(uint64_t all_pieces_bb, bool side){
		this->Pieces[side][ALL] = all_pieces_bb;
	}

	inline void setSidePiecesBB(bool side){
		this->Pieces[side][ALL] = this->Pieces[side][KING] | this->Pieces[side][QUEEN] | this->Pieces[side][ROOK] | this->Pieces[side][BISHOP] | this->Pieces[side][KNIGHT] | this->Pieces[side][PAWN];
	}
	inline void setKingPosBB(uint64_t king_bb, bool side){
		this->Pieces[side][KING] = king_bb;
	}

	inline void setQueensPosBB(uint64_t queens_bb, bool side){
		this->Pieces[side][QUEEN] = queens_bb;
	}
	inline void setKnightsPosBB(uint64_t knights_bb, bool side){
		this->Pieces[side][KNIGHT] = knights_bb;
	}

	inline void setPawnsPosBB(uint64_t pawns_bb, bool side){
		this->Pieces[side][PAWN] = pawns_bb;
	}

	inline void setBishopsPosBB(uint64_t bishops_bb, bool side){
		this->Pieces[side][BISHOP] = bishops_bb;
	}


	inline void setRooksPosBB(uint64_t rooks_bb, bool side){
		this->Pieces[side][ROOK] = rooks_bb;
	}

	inline uint64_t getMovesBB(bool side, int type){
		return this->attacks[side][type];
	}

	inline uint64_t getPiecesBB(bool side, int type){
		return this->Pieces[side][type];
	}
	
		
			

		PieceManager(void);
		PieceManager(PieceArgs args);
		void setBoard(PieceArgs args);
		
		void generateAllMoves(bool);
		void generateQueenMoves(bool);	
		void generatePawnMoves(bool);
		void generateKingMoves(bool);
		void generateKnightMoves(bool);
		void generateRookMoves(bool);
		void generateBishopMoves(bool);

		void clearMoves(bool);	
		void addPossibleMove(int, uint64_t, bool, int);
		void addKingSideCastlingRights(bool);
		void addQueenSideCastlingRights(bool);
		bool isPromoting(bool, int, int);	
		std::array<unsigned int, 256> getEncodedMove(void);	
		bool canQueenSideCastle(bool);
		bool canKingSideCastle(bool);
		bool isAttacked(bool, int);
		void addEnPassantRights(bool, int);
};

