
#include <iostream>
#include "pieces.h"
#include "types.h"
#include "utils.h"
#include "magic.h"

PieceManager::PieceManager(){
			
			Pieces[WHITE][KING] = W_KING_START;
			Pieces[WHITE][ROOK] = W_ROOKS_START;
			Pieces[WHITE][PAWN] = W_PAWNS_START;
			Pieces[WHITE][BISHOP] = W_BISHOPS_START;
			Pieces[WHITE][QUEEN] = W_QUEEN_START;
			Pieces[WHITE][KNIGHT] = W_KNIGHTS_START;

			
			Pieces[BLACK][KING] =  B_KING_START; 
			Pieces[BLACK][PAWN] = B_PAWNS_START;
			Pieces[BLACK][QUEEN] = B_QUEEN_START;
			Pieces[BLACK][BISHOP] = B_BISHOPS_START;
			Pieces[BLACK][ROOK] = B_ROOKS_START;
			Pieces[BLACK][KNIGHT] = B_KNIGHTS_START;
			
		        Pieces[BLACK][ALL] = B_ALL_PIECES_START;
			Pieces[WHITE][ALL] = W_ALL_PIECES_START;


}


PieceManager::PieceManager(PieceArgs& args){
			uint64_t w_all_pieces = args.w_king_bb | args.w_knights_bb | args.w_pawns_bb | args.w_rooks_bb | args.w_bishops_bb | args.w_queens_bb;

			uint64_t b_all_pieces = args.b_king_bb | args.b_knights_bb | args.b_pawns_bb | args.b_rooks_bb | args.b_bishops_bb |args.b_queens_bb;


			Pieces[WHITE][KING] = args.w_king_bb;
                        Pieces[WHITE][ROOK] = args.w_rooks_bb;
                        Pieces[WHITE][PAWN] = args.w_pawns_bb;
                        Pieces[WHITE][BISHOP] = args.w_bishops_bb;
                        Pieces[WHITE][QUEEN] = args.w_queens_bb;
                        Pieces[WHITE][KNIGHT] = args.w_knights_bb;


                        Pieces[BLACK][KING] =  args.b_king_bb;
                        Pieces[BLACK][PAWN] = args.b_pawns_bb;
                        Pieces[BLACK][QUEEN] = args.b_queens_bb;
                        Pieces[BLACK][BISHOP] = args.b_bishops_bb;
                        Pieces[BLACK][ROOK] = args.b_rooks_bb;
                        Pieces[BLACK][KNIGHT] = args.b_knights_bb;

                        Pieces[BLACK][ALL] = b_all_pieces;
                        Pieces[WHITE][ALL] = w_all_pieces;


}
void PieceManager::setBoard(PieceArgs& args){
 uint64_t w_all_pieces = args.w_king_bb | args.w_knights_bb | args.w_pawns_bb | args.w_rooks_bb | args.w_bishops_bb | args.w_queens_bb;                                                                                                                                                                                                                                                                                                                      uint64_t b_all_pieces = args.b_king_bb | args.b_knights_bb | args.b_pawns_bb | args.b_rooks_bb | args.b_bishops_bb |args.b_queens_bb;


                        Pieces[WHITE][KING] = args.w_king_bb;
                        Pieces[WHITE][ROOK] = args.w_rooks_bb;
                        Pieces[WHITE][PAWN] = args.w_pawns_bb;
                        Pieces[WHITE][BISHOP] = args.w_bishops_bb;
                        Pieces[WHITE][QUEEN] = args.w_queens_bb;
                        Pieces[WHITE][KNIGHT] = args.w_knights_bb;


                        Pieces[BLACK][KING] =  args.b_king_bb;
                        Pieces[BLACK][PAWN] = args.b_pawns_bb;
                        Pieces[BLACK][QUEEN] = args.b_queens_bb;
                        Pieces[BLACK][BISHOP] = args.b_bishops_bb;
                        Pieces[BLACK][ROOK] = args.b_rooks_bb;
                        Pieces[BLACK][KNIGHT] = args.b_knights_bb;

                        Pieces[BLACK][ALL] = b_all_pieces;
                        Pieces[WHITE][ALL] = w_all_pieces;

}


bool PieceManager::canKingSideCastle(bool side){
	if (side == WHITE)
		return (Pieces[side][KING] & bitset(E1)) && 
			(Pieces[side][ROOK] & bitset(H1)) && 
			((1ULL << F1) & ~(Pieces[WHITE][ALL] | Pieces[BLACK][ALL])) && 
			((1ULL << G1) & ~(Pieces[WHITE][ALL] | Pieces[BLACK][ALL])) && 
			!isAttacked(side, E1) && 
			!isAttacked(side, F1) && 
			!isAttacked(side, G1);
	else
		return (Pieces[side][KING] & bitset(E8)) &&                                                                                                                 (Pieces[side][ROOK] & bitset(H8)) &&                                                                                                                ((1ULL << F8) & ~(Pieces[WHITE][ALL] | Pieces[BLACK][ALL])) &&                                                                                      ((1ULL << G8) & ~(Pieces[WHITE][ALL] | Pieces[BLACK][ALL])) &&
                        !isAttacked(side, E8) &&
                        !isAttacked(side, F8) &&
                        !isAttacked(side, G8);
}

bool PieceManager::isAttacked(bool side, Square square){
	bool opponent = !side;	
	int bishopIndex = utils::generateMagicIndex((this->Pieces[side][ALL] | this->Pieces[!side][ALL]) & bishopOccupancyMasks[square], bishopMagics[square], square, 1);
	 	
	int rookIndex = utils::generateMagicIndex((this->Pieces[side][ALL] | this->Pieces[!side][ALL]) & rookOccupancyMasks[square], rookMagics[square], square, 0);
	return (kingLookups[square] & this->Pieces[opponent][KING]) ||
	       (knightLookups[square] & this->Pieces[opponent][KNIGHT]) ||
		(pawnAttackLookups[side][square] & this->Pieces[opponent][PAWN]) ||
		(rookMoveList[square][rookIndex] & this->Pieces[opponent][ROOK]) ||
		(bishopMoveList[square][bishopIndex] & this->Pieces[opponent][BISHOP]) ||
		((bishopMoveList[square][bishopIndex] | rookMoveList[square][rookIndex]) & this->Pieces[opponent][QUEEN]);
}

bool PieceManager::canQueenSideCastle(bool side){
	if (side == WHITE)
		return (Pieces[side][KING] & bitset(E1)) && 
			(Pieces[side][ROOK] & bitset(A1)) && 
			((1ULL << B1) & ~(Pieces[WHITE][ALL] | Pieces[BLACK][ALL])) && 
			((1ULL << C1) & ~(Pieces[WHITE][ALL] | Pieces[BLACK][ALL])) && 
			((1ULL << D1) & ~(Pieces[WHITE][ALL] | Pieces[BLACK][ALL])) &&  
			!isAttacked(side, C1) && 
			!isAttacked(side, D1) && 
			!isAttacked(side, E1);
	else
		return  (Pieces[side][KING] & bitset(E8)) &&
                        (Pieces[side][ROOK] & bitset(A8)) &&
                        ((1ULL << B8) & ~(Pieces[WHITE][ALL] | Pieces[BLACK][ALL])) &&
                        ((1ULL << C8) & ~(Pieces[WHITE][ALL] | Pieces[BLACK][ALL])) &&
                        ((1ULL << D8) & ~(Pieces[WHITE][ALL] | Pieces[BLACK][ALL])) &&
                        !isAttacked(side, C8) &&
                        !isAttacked(side, D8) &&
                        !isAttacked(side, E8);
}

