
#include <iostream>
#include "lookup.h"
#include "pieces.h"
#include "types.h"
#include <array>


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

			move_count = 0;
			generateAllMoves(WHITE);
			//generateAllMoves(BLACK);
}


PieceManager::PieceManager(PieceArgs args){
			uint64_t w_all_pieces = args.w_king_bb | args.w_knights_bb | args.w_pawns_bb | args.w_rooks_bb | args.w_bishops_bb | args.w_queens_bb;

			uint64_t b_all_pieces = args.b_king_bb | args.b_knights_bb | args.b_pawns_bb | args.b_rooks_bb | args.b_bishops_bb |args.b_queens_bb;


	uint64_t all_pieces_bb = w_all_pieces | b_all_pieces;
	//setAllPiecesBB(all_pieces_bb);
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
	
	move_count = 0;
	
	generateAllMoves(WHITE);

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
	
	move_count = 0;	
	generateAllMoves(WHITE);
}

void PieceManager::clearMoves(bool side){
	fill(begin(generatedMoves), end(generatedMoves), 0);
	move_count = 0;	

	
}



void PieceManager::generatePawnMoves(bool side){
	uint64_t bitboard = this->Pieces[side][PAWN];
	uint64_t pawnMoves = 0ULL;
	uint64_t captureMoves = 0ULL;
	int index;
	uint64_t shift;
	uint64_t all = this->Pieces[BLACK][ALL] | this->Pieces[WHITE][ALL];	
  
	while (bitboard != 0ULL){
		index = utils::countr_zero(bitboard);
			
		bitboard = bitclear(bitboard, index);
		if (side == WHITE)
			shift = ~all<<8;
		else 	
			shift = ~all>>8;

		pawnMoves |= (((pawnLookups[side][index])) & (~all | shift)) | (pawnAttackLookups[side][index] & this->Pieces[!side][ALL] );
		addPossibleMove(index, ((((pawnLookups[side][index]) ) & (~all | shift)) | (pawnAttackLookups[side][index] & this->Pieces[!side][ALL])), side, PAWN);
	}	
	this->attacks[side][PAWN] = pawnMoves;	
	this->attacks[side][ALL] |= this->attacks[side][PAWN]; 

}

void PieceManager::generateKingMoves(bool side){
	uint64_t bitboard = this->Pieces[side][KING];
 	uint64_t kingMoves = 0ULL;
 	
	while (bitboard != 0ULL){
		int index = utils::countr_zero(bitboard);
		bitboard = bitclear(bitboard, index);

		kingMoves |= kingLookups[index] & ~(this->Pieces[side][ALL]);
		addPossibleMove(index, kingLookups[index] & ~(this->Pieces[side][ALL]), side, KING);
 	}


 	this->attacks[side][KING] = kingMoves;
  	this->attacks[side][ALL] |= this->attacks[side][KING];	
 
}

void PieceManager::addKingSideCastlingRights(bool side){
	if (side == WHITE){
		this->attacks[side][KING] |= bitset(G1);
		generatedMoves[move_count] = encodeMove(E1, G1, CASTLE_FLAG);
	}
	else{
		this->attacks[side][KING] |= bitset(G8);
		generatedMoves[move_count] = encodeMove(E8, G8, CASTLE_FLAG);	
	      }
		
        move_count += 1;	
}

void PieceManager::addQueenSideCastlingRights(bool side){
	if (side == WHITE){
	       	this->attacks[side][KING] |= bitset(C1);
	        generatedMoves[move_count] = encodeMove(E1, C1, CASTLE_FLAG);
	}
	else{
		this->attacks[side][KING] |= bitset(C8);
		generatedMoves[move_count] = encodeMove(E8, C8, CASTLE_FLAG);
	}

	move_count += 1;
}

void PieceManager::generateRookMoves(bool side){
	uint64_t bitboard = this->Pieces[side][ROOK];
 	uint64_t rookMoves = 0ULL;

	while (bitboard != 0ULL){
		int square = utils::countr_zero(bitboard);

		
		int index = utils::generateMagicIndex((this->Pieces[side][ALL] | this->Pieces[!side][ALL]) & rookOccupancyMasks[square], rookMagics[square], square, 0);
	
		rookMoves |= rookMoveList[square][index] & ~(this->Pieces[side][ALL]);
		bitboard = bitclear(bitboard, square);

		addPossibleMove(index, rookMoveList[square][index] & ~(this->Pieces[side][ALL]), side, ROOK);
 	}

	this->attacks[side][ROOK] = rookMoves;
	
	this->attacks[side][ALL] |= this->attacks[side][ROOK]; 
	
}


void PieceManager::generateQueenMoves(bool side){
	uint64_t bitboard = this->Pieces[side][QUEEN];
 	uint64_t rookMoves = 0ULL;
	uint64_t bishopMoves = 0ULL;

	while (bitboard != 0ULL){
		int square = utils::countr_zero(bitboard);

		
		int rookIndex = utils::generateMagicIndex((this->Pieces[side][ALL] | this->Pieces[!side][ALL]) & rookOccupancyMasks[square], rookMagics[square], square, 0);
	
		rookMoves |= rookMoveList[square][rookIndex] & ~(this->Pieces[side][ALL]);
		int bishopIndex = utils::generateMagicIndex((this->Pieces[side][ALL] | this->Pieces[!side][ALL]) & bishopOccupancyMasks[square], bishopMagics[square], square, 1);

		bishopMoves |= bishopMoveList[square][bishopIndex] & ~(this->Pieces[side][ALL]);
		bitboard = bitclear(bitboard, square);

		addPossibleMove(square, (bishopMoveList[square][bishopIndex] & ~(this->Pieces[side][ALL])) | (rookMoveList[square][rookIndex] & ~(this->Pieces[side][ALL])), side, QUEEN);
	}

	this->attacks[side][QUEEN] = (bishopMoves | rookMoves);
	
	this->attacks[side][ALL] |= this->attacks[side][QUEEN]; 
	
}
void PieceManager::generateBishopMoves(bool side){
	uint64_t bitboard = this->Pieces[side][BISHOP];
 	uint64_t bishopMoves = 0ULL;
 	
	while (bitboard != 0ULL){
	
		int square = utils::countr_zero(bitboard);

		
		int index = utils::generateMagicIndex((this->Pieces[side][ALL] | this->Pieces[!side][ALL]) & bishopOccupancyMasks[square], bishopMagics[square], square, 1);
	
		bishopMoves |= bishopMoveList[square][index] & ~(this->Pieces[side][ALL]);
		bitboard = bitclear(bitboard, square);

		addPossibleMove(square,bishopMoveList[square][index] & ~(this->Pieces[side][ALL]), side, BISHOP);
 	}

	this->attacks[side][BISHOP] = bishopMoves;
	this->attacks[side][ALL] |= this->attacks[side][BISHOP]; 
	
}

void PieceManager::generateKnightMoves(bool side){
 uint64_t bitboard = this->Pieces[side][KNIGHT]; 
 uint64_t knightMoves = 0ULL;

 while (bitboard != 0ULL){
	int index = utils::countr_zero(bitboard);
	bitboard = bitclear(bitboard, index);

	knightMoves |= knightLookups[index] & ~(this->Pieces[side][ALL]);

	addPossibleMove(index, knightLookups[index] & ~(this->Pieces[side][ALL]), side, KNIGHT);
 }

 this->attacks[side][KNIGHT] = knightMoves;
 this->attacks[side][ALL] |= this->attacks[side][KNIGHT];

}

void PieceManager::generateAllMoves(bool side){
			this->attacks[side][ALL] = 0ULL;
			generateKingMoves(side);
			generateQueenMoves(side);
			generateRookMoves(side);
			generateBishopMoves(side);
			generateKnightMoves(side);
			generatePawnMoves(side);	
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
unsigned int PieceManager::encodeMove(int from, int to, int flags){

	return  ((flags & 0xf)<<12) | ((from & 0x3f)<<6) | (to & 0x3f);
}
void PieceManager::addPossibleMove(int start, uint64_t positions, bool side, int type){

	while (positions != 0ULL){
		int to = utils::countr_zero(positions);
	
		if (type == PAWN && isPromoting(side, start, to)){
			for(const auto& promoted_code: promoted_piece_codes){
				generatedMoves[move_count] = encodeMove(start, to, promoted_code);
				move_count += 1;

			}
		}	

		else{	
			generatedMoves[move_count] = encodeMove(start, to, 0ULL);	
			move_count += 1;	
		}
		positions = bitclear(positions, to);

	}
}

std::array<unsigned int, 256> PieceManager::getEncodedMove(){
	return this->generatedMoves;
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

