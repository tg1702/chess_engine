
#include <iostream>
#include <cstdint>
#include "pieces.h"
#include "types.h"
#include "./movegen/magic.h"

PieceSet::PieceSet(){
			
			Pieces[Side::WHITE][KING] = W_KING_START;
			Pieces[Side::WHITE][ROOK] = W_ROOKS_START;
			Pieces[Side::WHITE][PAWN] = W_PAWNS_START;
			Pieces[Side::WHITE][BISHOP] = W_BISHOPS_START;
			Pieces[Side::WHITE][QUEEN] = W_QUEEN_START;
			Pieces[Side::WHITE][KNIGHT] = W_KNIGHTS_START;

			
			Pieces[Side::BLACK][KING] =  B_KING_START; 
			Pieces[Side::BLACK][PAWN] = B_PAWNS_START;
			Pieces[Side::BLACK][QUEEN] = B_QUEEN_START;
			Pieces[Side::BLACK][BISHOP] = B_BISHOPS_START;
			Pieces[Side::BLACK][ROOK] = B_ROOKS_START;
			Pieces[Side::BLACK][KNIGHT] = B_KNIGHTS_START;
			
		    Pieces[Side::BLACK][ALL] = B_ALL_PIECES_START;
			Pieces[Side::WHITE][ALL] = W_ALL_PIECES_START;


}


PieceSet::PieceSet(const PieceBB& w_args, const PieceBB& b_args){
	Bitboard w_all_pieces = w_args.king_bb | w_args.knight_bb | w_args.pawn_bb | w_args.rook_bb | w_args.bishop_bb | b_args.queen_bb;

	Bitboard b_all_pieces = b_args.king_bb | b_args.knight_bb | b_args.pawn_bb | b_args.rook_bb | b_args.bishop_bb |b_args.queen_bb;


	Pieces[Side::WHITE][KING] = w_args.king_bb;
	Pieces[Side::WHITE][ROOK] = w_args.rook_bb;
	Pieces[Side::WHITE][PAWN] = w_args.pawn_bb;
	Pieces[Side::WHITE][BISHOP] = w_args.bishop_bb;
	Pieces[Side::WHITE][QUEEN] = w_args.queen_bb;
	Pieces[Side::WHITE][KNIGHT] = w_args.knight_bb;


	Pieces[Side::BLACK][KING] =  b_args.king_bb;
	Pieces[Side::BLACK][PAWN] = b_args.pawn_bb;
	Pieces[Side::BLACK][QUEEN] = b_args.queen_bb;
	Pieces[Side::BLACK][BISHOP] = b_args.bishop_bb;
	Pieces[Side::BLACK][ROOK] = b_args.rook_bb;
	Pieces[Side::BLACK][KNIGHT] = b_args.knight_bb;

	Pieces[Side::BLACK][ALL] = b_all_pieces;
	Pieces[Side::WHITE][ALL] = w_all_pieces;


}
void PieceSet::setBoard(const PieceBB& w_args, const PieceBB& b_args){
Bitboard w_all_pieces = w_args.king_bb | w_args.knight_bb | w_args.pawn_bb | w_args.rook_bb | w_args.bishop_bb | b_args.queen_bb;

	Bitboard b_all_pieces = b_args.king_bb | b_args.knight_bb | b_args.pawn_bb | b_args.rook_bb | b_args.bishop_bb |b_args.queen_bb;


	Pieces[Side::WHITE][KING] = w_args.king_bb;
	Pieces[Side::WHITE][ROOK] = w_args.rook_bb;
	Pieces[Side::WHITE][PAWN] = w_args.pawn_bb;
	Pieces[Side::WHITE][BISHOP] = w_args.bishop_bb;
	Pieces[Side::WHITE][QUEEN] = w_args.queen_bb;
	Pieces[Side::WHITE][KNIGHT] = w_args.knight_bb;


	Pieces[Side::BLACK][KING] =  b_args.king_bb;
	Pieces[Side::BLACK][PAWN] = b_args.pawn_bb;
	Pieces[Side::BLACK][QUEEN] = b_args.queen_bb;
	Pieces[Side::BLACK][BISHOP] = b_args.bishop_bb;
	Pieces[Side::BLACK][ROOK] = b_args.rook_bb;
	Pieces[Side::BLACK][KNIGHT] = b_args.knight_bb;

	Pieces[Side::BLACK][ALL] = b_all_pieces;
	Pieces[Side::WHITE][ALL] = w_all_pieces;
}


bool PieceSet::canKingSideCastle(Side side){
	if (side == Side::WHITE)
		return (Pieces[side][KING] & bitset(E1)) && 
			(Pieces[side][ROOK] & bitset(H1)) && 
			((1ULL << F1) & ~(Pieces[Side::WHITE][ALL] | Pieces[Side::BLACK][ALL])) && 
			((1ULL << G1) & ~(Pieces[Side::WHITE][ALL] | Pieces[Side::BLACK][ALL])) && 
			!isAttacked(side, E1) && 
			!isAttacked(side, F1) && 
			!isAttacked(side, G1);
	else
		return (Pieces[side][KING] & bitset(E8)) &&                                                                                                                 (Pieces[side][ROOK] & bitset(H8)) &&                                                                                                                ((1ULL << F8) & ~(Pieces[Side::WHITE][ALL] | Pieces[Side::BLACK][ALL])) &&                                                                                      ((1ULL << G8) & ~(Pieces[Side::WHITE][ALL] | Pieces[Side::BLACK][ALL])) &&
                        !isAttacked(side, E8) &&
                        !isAttacked(side, F8) &&
                        !isAttacked(side, G8);
}

bool PieceSet::isAttacked(Side side, Square square){
	Side opponent = !side;	
	int bishopIndex = utils::generateMagicIndex((this->Pieces[side][ALL] | this->Pieces[opponent][ALL]) & bishopOccupancyMasks[square], bishopMagics[square], square, 1);
	 	
	int rookIndex = utils::generateMagicIndex((this->Pieces[side][ALL] | this->Pieces[opponent][ALL]) & rookOccupancyMasks[square], rookMagics[square], square, 0);
	
	if (this->Pieces[opponent][PAWN] & pawnAttackLookups[side][square]) 
		return true;
	if (this->Pieces[opponent][KNIGHT] & knightLookups[square])	
		return true;
	if (this->Pieces[opponent][KING] & kingLookups[square])
		return true;
	if (this->Pieces[opponent][ROOK] & rookMoveList[square][rookIndex])
		return true;
	if (this->Pieces[opponent][BISHOP] & bishopMoveList[square][bishopIndex])
		return true;
	if ( (bishopMoveList[square][bishopIndex] | rookMoveList[square][rookIndex]) & this->Pieces[opponent][QUEEN])
		return true;

	return false;
}

bool PieceSet::canQueenSideCastle(Side side){
	if (side == Side::WHITE)
		return (Pieces[side][KING] & bitset(E1)) && 
			(Pieces[side][ROOK] & bitset(A1)) && 
			((1ULL << B1) & ~(Pieces[Side::WHITE][ALL] | Pieces[Side::BLACK][ALL])) && 
			((1ULL << C1) & ~(Pieces[Side::WHITE][ALL] | Pieces[Side::BLACK][ALL])) && 
			((1ULL << D1) & ~(Pieces[Side::WHITE][ALL] | Pieces[Side::BLACK][ALL])) &&  
			!isAttacked(side, C1) && 
			!isAttacked(side, D1) && 
			!isAttacked(side, E1);
	else
		return  (Pieces[side][KING] & bitset(E8)) &&
                        (Pieces[side][ROOK] & bitset(A8)) &&
                        ((1ULL << B8) & ~(Pieces[Side::WHITE][ALL] | Pieces[Side::BLACK][ALL])) &&
                        ((1ULL << C8) & ~(Pieces[Side::WHITE][ALL] | Pieces[Side::BLACK][ALL])) &&
                        ((1ULL << D8) & ~(Pieces[Side::WHITE][ALL] | Pieces[Side::BLACK][ALL])) &&
                        !isAttacked(side, C8) &&
                        !isAttacked(side, D8) &&
                        !isAttacked(side, E8);
}
