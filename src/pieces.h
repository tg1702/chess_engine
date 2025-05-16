
#ifndef PIECES_H
#define PIECES_H

#include <iostream>
#include "types.h"
#include "utils.h"

#define SIDES 2
#define MAX_PIECE_TYPES 7



class PieceSet{

	public:
		inline void setAnyPosBB(Side side, PieceType type, Bitboard pos){
			this->Pieces[side][type] = pos;
		}

		inline int getPieceCount(Side side, PieceType type) const{
			return __builtin_popcountll(this->Pieces[side][type]);
		}

		inline void setSidePiecesBB(const Bitboard all_pieces_bb, Side side){
			this->Pieces[side][ALL] = all_pieces_bb;
			}

		inline void updatePiecesBB(Side side, PieceType type, const Bitboard bb){
			this->Pieces[side][type] = bb;
		}

		inline void setSidePiecesBB(Side side){
			this->Pieces[side][ALL] = this->Pieces[side][KING] | this->Pieces[side][QUEEN] | this->Pieces[side][ROOK] | this->Pieces[side][BISHOP] | this->Pieces[side][KNIGHT] | this->Pieces[side][PAWN];
		}


		inline Bitboard getPiecesBB(Side side, PieceType type) const{
			return this->Pieces[side][type];
		}
		
			
		inline void movePiece(Side side, PieceType type, Square from, Square to){
			this->Pieces[side][type] ^= (bitset(from)  |  bitset(to));
		}

		inline void clearPiece(Side side, PieceType type, Square square){
			bitclear(this->Pieces[side][type], square);
		
		}

		inline void addPiece(Side side, PieceType type, Square on){
			this->Pieces[side][type] |= bitset(on);
		}

		PieceSet(void);
		PieceSet(const PieceBB& w_args, const PieceBB& b_args);
		void setBoard(const PieceBB& w_args, const PieceBB& b_args);
		
		bool canKingSideCastle(Side);
		bool canQueenSideCastle(Side);		
		bool isAttacked(Side, Square);

	private:
		Bitboard Pieces[SIDES][MAX_PIECE_TYPES];
};

#endif