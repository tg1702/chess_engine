#include <iostream>
#include "types.h"
#include "magic.h"
#include "lookup.h"

#define SIDES 2
#define PIECE_TYPES 7
using namespace std;

#pragma once
class PieceManager{
	private:
		uint64_t Pieces[SIDES][PIECE_TYPES];
		uint64_t attacks[SIDES][PIECE_TYPES];
		uint64_t all_pieces;
	public:
		PieceManager(){
			
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
		void setBoard(PieceArgs args);
		void setAllPieces(uint64_t);
		void setSidePieces(uint64_t, bool);
		void setKingPos(uint64_t, bool);
		void setKnightsPos(uint64_t, bool);
		void setPawnsPos(uint64_t, bool);
		void setRooksPos(uint64_t, bool);
		void setBishopsPos(uint64_t, bool);
		void setQueensPos(uint64_t, bool);

	
		uint64_t getQueenMoves(bool);	
		uint64_t getPawnMoves(bool);
		uint64_t getKingMoves(bool);
		uint64_t getKnightMoves(bool);
		uint64_t getRookMoves(bool);
		uint64_t getBishopMoves(bool);
		uint64_t outputRank(uint64_t);
		void makeMove(int, int, bool);
};

void PieceManager::setBoard(PieceArgs args){
	uint64_t w_all_pieces = args.w_king_bb | args.w_knights_bb | args.w_pawns_bb | args.w_rooks_bb | args.w_bishops_bb | args.w_queens_bb;

	uint64_t b_all_pieces = args.b_king_bb | args.b_knights_bb | args.b_pawns_bb | args.b_rooks_bb | args.b_bishops_bb |args.b_queens_bb;


	uint64_t all_pieces_bb = w_all_pieces | b_all_pieces;
	setAllPieces(all_pieces_bb);
	setSidePieces(w_all_pieces, WHITE);
	setSidePieces(b_all_pieces, BLACK);
	
	setQueensPos(args.w_queens_bb, WHITE);
	setBishopsPos(args.w_bishops_bb, WHITE);
	setKingPos(args.w_king_bb, WHITE);
	setKnightsPos(args.w_knights_bb, WHITE);
	setPawnsPos(args.w_pawns_bb, WHITE);
	setRooksPos(args.w_rooks_bb, WHITE);
	
	
	setKingPos(args.b_king_bb, BLACK);
	setKnightsPos(args.b_knights_bb, BLACK);
	setPawnsPos(args.b_pawns_bb, BLACK);
	setQueensPos(args.b_queens_bb, BLACK);
	setBishopsPos(args.b_bishops_bb, BLACK);
	setRooksPos(args.b_rooks_bb, BLACK);	
}

void PieceManager::setAllPieces(uint64_t all_pieces_bb){
	this->all_pieces = all_pieces_bb;
}

void PieceManager::setSidePieces(uint64_t all_pieces_bb, bool side){
	this->Pieces[side][ALL] = all_pieces_bb;
}

void PieceManager::setKingPos(uint64_t king_bb, bool side){
	this->Pieces[side][KING] = king_bb;
}

void PieceManager::setQueensPos(uint64_t queens_bb, bool side){
	this->Pieces[side][QUEEN] = queens_bb;
}
void PieceManager::setKnightsPos(uint64_t knights_bb, bool side){
	this->Pieces[side][KNIGHT] = knights_bb;
}

void PieceManager::setPawnsPos(uint64_t pawns_bb, bool side){
	this->Pieces[side][PAWN] = pawns_bb;
}

void PieceManager::setBishopsPos(uint64_t bishops_bb, bool side){
	this->Pieces[side][BISHOP] = bishops_bb;
}


void PieceManager::setRooksPos(uint64_t rooks_bb, bool side){
	this->Pieces[side][ROOK] = rooks_bb;
}

uint64_t PieceManager::getPawnMoves(bool side){
	uint64_t bitboard = this->Pieces[side][PAWN];
	uint64_t pawnMoves = 0ULL;
	uint64_t captureMoves = 0ULL;
	int index;
	
  	while (bitboard != 0ULL){
		index = countr_zero(bitboard);
		
		if (side == WHITE){
			captureMoves = (bitboard << 7 | bitboard << 9) & Pieces[!side][ALL] & outputRank(bitset(index) << 8);

		}
		else {
			captureMoves = (bitboard >> 7 | bitboard >> 9) & Pieces[!side][ALL] & outputRank(bitset(index) >> 8);
		}

			
		bitboard = bitclear(bitboard, index);
		pawnMoves |= pawnLookups[side][index] | captureMoves;
	}	
	this->attacks[side][PAWN] = (pawnMoves & ~all_pieces) | (pawnMoves & ~all_pieces<<8);
	return this->attacks[side][PAWN];
}

uint64_t PieceManager::getKingMoves(bool side){
	uint64_t bitboard = this->Pieces[side][KING];
 	uint64_t kingMoves = 0ULL;
 	
	while (bitboard != 0ULL){
		int index = countr_zero(bitboard);
		bitboard = bitclear(bitboard, index);

		kingMoves |= whiteKingLookups[index];
 	}


 	this->attacks[side][KING] = kingMoves & ~(this->Pieces[side][ALL]);
 	return this->attacks[side][KING];
 
}

uint64_t PieceManager::getRookMoves(bool side){
	uint64_t bitboard = this->Pieces[side][ROOK];
 	uint64_t rookMoves = 0ULL;

	while (bitboard != 0ULL){
		int square = countr_zero(bitboard);

		
		int index = generateMagicIndex((all_pieces) & rookOccupancyMasks[square], rookMagics[square], square, 0);
	
		rookMoves |= rookMoveList[square][index];

		bitboard = bitclear(bitboard, square);

 	}

	this->attacks[side][ROOK] = rookMoves & ~(this->Pieces[side][ALL]);
	return this->attacks[side][ROOK];
	
}


uint64_t PieceManager::getQueenMoves(bool side){
	uint64_t bitboard = this->Pieces[side][QUEEN];
 	uint64_t rookMoves = 0ULL;
	uint64_t bishopMoves = 0ULL;

	while (bitboard != 0ULL){
		int square = countr_zero(bitboard);

		
		int rookIndex = generateMagicIndex((all_pieces) & rookOccupancyMasks[square], rookMagics[square], square, 0);
	
		rookMoves |= rookMoveList[square][rookIndex];
		int bishopIndex = generateMagicIndex((all_pieces) & bishopOccupancyMasks[square], bishopMagics[square], square, 1);

		bishopMoves |= bishopMoveList[square][rookIndex];
		bitboard = bitclear(bitboard, square);

 	}

	this->attacks[side][QUEEN] = (bishopMoves | rookMoves) & ~(this->Pieces[side][ALL]);
	return this->attacks[side][QUEEN];
	
}
uint64_t PieceManager::getBishopMoves(bool side){
	uint64_t bitboard = this->Pieces[side][BISHOP];
 	uint64_t bishopMoves = 0ULL;
 	
	while (bitboard != 0ULL){
	
		int square = countr_zero(bitboard);

		
		int index = generateMagicIndex((all_pieces) & bishopOccupancyMasks[square], bishopMagics[square], square, 1);
	
		bishopMoves |= bishopMoveList[square][index];

		bitboard = bitclear(bitboard, square);

 	}

	this->attacks[side][BISHOP] = bishopMoves & ~(this->Pieces[side][ALL]);
	return this->attacks[side][BISHOP];
	
}

void PieceManager::makeMove(int from, int to, bool side){
	bool validCapture = false;
	for (int i = 0; i < PIECE_TYPES; i++){
	
		if ((this->Pieces[side][i] & bitset(from)) || (this->attacks[side][i] & bitset(to))){
			this->Pieces[side][i] ^= bitset(from);
			this->Pieces[side][i] ^= bitset(to);
			validCapture = true;	
		}

		if (validCapture && (this->Pieces[side][i] & bitset(from)))
			this->Pieces[side][i] ^= bitset(to);

	}
		
}
uint64_t PieceManager::getKnightMoves(bool side){
 uint64_t knights = this->Pieces[side][KNIGHT]; 
 uint64_t twoRowAheadLocation = outputRank(knights << 16);
 uint64_t oneRowAheadLocation = outputRank(knights << 8);

 uint64_t twoRowBehindLocation = outputRank(knights >> 16);
 uint64_t oneRowBehindLocation = outputRank(knights >> 8);
 uint64_t forwardKnightMoves = (((knights << 15) | (knights << 17)) & twoRowAheadLocation) | (((knights << 6) | (knights << 10)) & oneRowAheadLocation); 
 uint64_t backwardKnightMoves = (((knights >> 15) | (knights >> 17)) & twoRowBehindLocation) | (((knights >> 6) | (knights >> 10)) & oneRowBehindLocation);

 
 uint64_t knightMoves = forwardKnightMoves | backwardKnightMoves;

 return  knightMoves & ~(this->Pieces[side][ALL]);
}

uint64_t PieceManager::outputRank(uint64_t pos){
	uint64_t rank_location = 0;
	for (int i = 0; i < 8; ++i){
		if (ranks[i] & pos)
			rank_location = rank_location | ranks[i];
	}
	return rank_location;
}

