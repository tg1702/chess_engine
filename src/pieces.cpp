
#include <iostream>
#include "lookup.h"
#include "pieces.h"
#include "types.h"
#include "move.h"
#include <vector>
#include <algorithm>

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

			all_pieces = Pieces[BLACK][ALL] | Pieces[WHITE][ALL];

}


PieceManager::PieceManager(PieceArgs args){
			uint64_t w_all_pieces = args.w_king_bb | args.w_knights_bb | args.w_pawns_bb | args.w_rooks_bb | args.w_bishops_bb | args.w_queens_bb;

			uint64_t b_all_pieces = args.b_king_bb | args.b_knights_bb | args.b_pawns_bb | args.b_rooks_bb | args.b_bishops_bb |args.b_queens_bb;


	uint64_t all_pieces_bb = w_all_pieces | b_all_pieces;
	setSidePiecesBB(w_all_pieces, WHITE);
	setSidePiecesBB(b_all_pieces, BLACK);
	
	setQueensPosBB(args.w_queens_bb, WHITE);
	setBishopsPosBB(args.w_bishops_bb, WHITE);
	setKingPosBB(args.w_king_bb, WHITE);
	setKnightsPosBB(args.w_knights_bb, WHITE);
	setPawnsPosBB(args.w_pawns_bb, WHITE);
	setRooksPosBB(args.w_rooks_bb, WHITE);
	
	
	setKingPosBB(args.b_king_bb, BLACK);
	setKnightsPosBB(args.b_knights_bb, BLACK);
	setPawnsPosBB(args.b_pawns_bb, BLACK);
	setQueensPosBB(args.b_queens_bb, BLACK);
	setBishopsPosBB(args.b_bishops_bb, BLACK);
	setRooksPosBB(args.b_rooks_bb, BLACK);
	
	
	//generateAllMoves(WHITE);

}

void PieceManager::setBoard(PieceArgs args){

	uint64_t w_all_pieces = args.w_king_bb | args.w_knights_bb | args.w_pawns_bb | args.w_rooks_bb | args.w_bishops_bb | args.w_queens_bb;

	uint64_t b_all_pieces = args.b_king_bb | args.b_knights_bb | args.b_pawns_bb | args.b_rooks_bb | args.b_bishops_bb |args.b_queens_bb;


	uint64_t all_pieces_bb = w_all_pieces | b_all_pieces;
	setAllPiecesBB(all_pieces_bb);
	setSidePiecesBB(w_all_pieces, WHITE);
	setSidePiecesBB(b_all_pieces, BLACK);
	
	setQueensPosBB(args.w_queens_bb, WHITE);
	setBishopsPosBB(args.w_bishops_bb, WHITE);
	setKingPosBB(args.w_king_bb, WHITE);
	setKnightsPosBB(args.w_knights_bb, WHITE);
	setPawnsPosBB(args.w_pawns_bb, WHITE);
	setRooksPosBB(args.w_rooks_bb, WHITE);
	
	
	setKingPosBB(args.b_king_bb, BLACK);
	setKnightsPosBB(args.b_knights_bb, BLACK);
	setPawnsPosBB(args.b_pawns_bb, BLACK);
	setQueensPosBB(args.b_queens_bb, BLACK);
	setBishopsPosBB(args.b_bishops_bb, BLACK);
	setRooksPosBB(args.b_rooks_bb, BLACK);
	
}




bool PieceManager::isPromoting(bool side, int from, int to){

		if (side == WHITE && (bitset(from) & RANK_7) && (bitset(to) & RANK_8)){
			return true;
		}
		else if (side == BLACK && (bitset(from) & RANK_2) && (bitset(to) & RANK_1)){
			return true;
		}
		else{
			return false;
		}

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

bool PieceManager::isAttacked(bool side, int square){
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
			!isAttacked(side, B1) && 
			!isAttacked(side, C1) && 
			!isAttacked(side, D1) && 
			!isAttacked(side, E1);
	else
		return  (Pieces[side][KING] & bitset(E8)) &&
                        (Pieces[side][ROOK] & bitset(A8)) &&
                        ((1ULL << B8) & ~(Pieces[WHITE][ALL] | Pieces[BLACK][ALL])) &&
                        ((1ULL << C8) & ~(Pieces[WHITE][ALL] | Pieces[BLACK][ALL])) &&
                        ((1ULL << D8) & ~(Pieces[WHITE][ALL] | Pieces[BLACK][ALL])) &&
                        !isAttacked(side, B8) &&
                        !isAttacked(side, C8) &&
                        !isAttacked(side, D8) &&
                        !isAttacked(side, E8);
}

void PieceManager::movePiece(bool side, int type, int from, int to){
	clearPiece(side, type, from);
	addPiece(side, type, to);
}

void PieceManager::clearPiece(bool side, int type, int square){
	uint64_t curBB = getPiecesBB(side, type);
	bitclear(curBB, square);
	setAnyPosBB( side, type , curBB);
	
}

void PieceManager::addPiece(bool side, int type, int on){
	setAnyPosBB(side, type, getPiecesBB(side, type) | bitset(on));
}
