
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
		
		int generatedMoves[256] = {0ULL};
		int move_count;
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

			move_count = 0;

			generateKingMoves(WHITE);
			generateQueenMoves(WHITE);
			generateRookMoves(WHITE);
			generateBishopMoves(WHITE);
			generateKnightMoves(WHITE);
			generatePawnMoves(WHITE);


			generateKingMoves(BLACK);
			generateQueenMoves(BLACK);
			generateRookMoves(BLACK);
		 	generateBishopMoves(BLACK);
			generateKnightMoves(BLACK);
			generatePawnMoves(BLACK);	
		}
		void setBoard(PieceArgs args);
		void setAllPiecesBB(uint64_t);
		void setSidePiecesBB(uint64_t, bool);
		void setKingPosBB(uint64_t, bool);
		void setKnightsPosBB(uint64_t, bool);
		void setPawnsPosBB(uint64_t, bool);
		void setRooksPosBB(uint64_t, bool);
		void setBishopsPosBB(uint64_t, bool);
		void setQueensPosBB(uint64_t, bool);

		void setAnyPosBB(bool,int, uint64_t);	
		void generateQueenMoves(bool);	
		void generatePawnMoves(bool);
		void generateKingMoves(bool);
		void generateKnightMoves(bool);
		void generateRookMoves(bool);
		void generateBishopMoves(bool);

		uint64_t getMovesBB(bool, int);
		uint64_t getPiecesBB(bool, int);			
		
		uint64_t outputRank(uint64_t);

		
		void addPossibleMove(int, uint64_t);
		void addKingSideCastlingRights(bool);
		int encodeMove(int, int, int);
		int getEncodedMove(void);	
		bool canQueenSideCastle(bool);
		bool canKingSideCastle(bool);
		bool isAttacked(bool, int);
};

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

	generateKingMoves(WHITE);
	generateQueenMoves(WHITE);
	generateRookMoves(WHITE);
	generateBishopMoves(WHITE);
	generateKnightMoves(WHITE);
	generatePawnMoves(WHITE);


	generateKingMoves(BLACK);
	generateQueenMoves(BLACK);
	generateRookMoves(BLACK);
	generateBishopMoves(BLACK);
	generateKnightMoves(BLACK);
	generatePawnMoves(BLACK);	
}

void PieceManager::setAnyPosBB(bool side, int type, uint64_t pos){
	this->Pieces[side][type] = pos;
}
void PieceManager::setAllPiecesBB(uint64_t all_pieces_bb){
	this->all_pieces = all_pieces_bb;
}

void PieceManager::setSidePiecesBB(uint64_t all_pieces_bb, bool side){
	this->Pieces[side][ALL] = all_pieces_bb;
}

void PieceManager::setKingPosBB(uint64_t king_bb, bool side){
	this->Pieces[side][KING] = king_bb;
}

void PieceManager::setQueensPosBB(uint64_t queens_bb, bool side){
	this->Pieces[side][QUEEN] = queens_bb;
}
void PieceManager::setKnightsPosBB(uint64_t knights_bb, bool side){
	this->Pieces[side][KNIGHT] = knights_bb;
}

void PieceManager::setPawnsPosBB(uint64_t pawns_bb, bool side){
	this->Pieces[side][PAWN] = pawns_bb;
}

void PieceManager::setBishopsPosBB(uint64_t bishops_bb, bool side){
	this->Pieces[side][BISHOP] = bishops_bb;
}


void PieceManager::setRooksPosBB(uint64_t rooks_bb, bool side){
	this->Pieces[side][ROOK] = rooks_bb;
}

uint64_t PieceManager::getMovesBB(bool side, int type){
	return this->attacks[side][type];
}

uint64_t PieceManager::getPiecesBB(bool side, int type){
	return this->Pieces[side][type];
}
void PieceManager::generatePawnMoves(bool side){
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
		pawnMoves |= (pawnLookups[side][index] | captureMoves) & (~all_pieces | ~all_pieces<<8);

		addPossibleMove(index, (pawnLookups[side][index] | captureMoves) & (~all_pieces | ~all_pieces<<8));
		
	}	
	this->attacks[side][PAWN] = pawnMoves;	
	this->attacks[side][ALL] |= this->attacks[side][PAWN]; 

}

void PieceManager::generateKingMoves(bool side){
	uint64_t bitboard = this->Pieces[side][KING];
 	uint64_t kingMoves = 0ULL;
 	
	while (bitboard != 0ULL){
		int index = countr_zero(bitboard);
		bitboard = bitclear(bitboard, index);

		kingMoves |= whiteKingLookups[index] & ~(this->Pieces[side][ALL]);
		addPossibleMove(index, whiteKingLookups[index] & ~(this->Pieces[side][ALL]));
 	}


 	this->attacks[side][KING] = kingMoves;
  	this->attacks[side][ALL] |= this->attacks[side][KING];	
 
}

void PieceManager::addKingSideCastlingRights(bool side){
	side == WHITE? this->attacks[side][KING] | bitset(H1) : this->attacks[side][KING] | bitset(H8); 
}

void PieceManager::generateRookMoves(bool side){
	uint64_t bitboard = this->Pieces[side][ROOK];
 	uint64_t rookMoves = 0ULL;

	while (bitboard != 0ULL){
		int square = countr_zero(bitboard);

		
		int index = generateMagicIndex((all_pieces) & rookOccupancyMasks[square], rookMagics[square], square, 0);
	
		rookMoves |= rookMoveList[square][index] & ~(this->Pieces[side][ALL]);
		bitboard = bitclear(bitboard, square);

		addPossibleMove(index, rookMoveList[square][index] & ~(this->Pieces[side][ALL]));
 	}

	this->attacks[side][ROOK] = rookMoves;
	
	this->attacks[side][ALL] |= this->attacks[side][ROOK]; 
	
}


void PieceManager::generateQueenMoves(bool side){
	uint64_t bitboard = this->Pieces[side][QUEEN];
 	uint64_t rookMoves = 0ULL;
	uint64_t bishopMoves = 0ULL;

	while (bitboard != 0ULL){
		int square = countr_zero(bitboard);

		
		int rookIndex = generateMagicIndex((all_pieces) & rookOccupancyMasks[square], rookMagics[square], square, 0);
	
		rookMoves |= rookMoveList[square][rookIndex] & ~(this->Pieces[side][ALL]);
		int bishopIndex = generateMagicIndex((all_pieces) & bishopOccupancyMasks[square], bishopMagics[square], square, 1);

		bishopMoves |= bishopMoveList[square][rookIndex] & ~(this->Pieces[side][ALL]);
		bitboard = bitclear(bitboard, square);

		addPossibleMove(square, (bishopMoveList[square][rookIndex] & ~(this->Pieces[side][ALL])) | rookMoveList[square][rookIndex] & ~(this->Pieces[side][ALL]));
	}

	this->attacks[side][QUEEN] = (bishopMoves | rookMoves);
	
	this->attacks[side][ALL] |= this->attacks[side][QUEEN]; 
	
}
void PieceManager::generateBishopMoves(bool side){
	uint64_t bitboard = this->Pieces[side][BISHOP];
 	uint64_t bishopMoves = 0ULL;
 	
	while (bitboard != 0ULL){
	
		int square = countr_zero(bitboard);

		
		int index = generateMagicIndex((all_pieces) & bishopOccupancyMasks[square], bishopMagics[square], square, 1);
	
		bishopMoves |= bishopMoveList[square][index] & ~(this->Pieces[side][ALL]);
		bitboard = bitclear(bitboard, square);


		addPossibleMove(index,bishopMoveList[square][index] & ~(this->Pieces[side][ALL]));
 	}

	this->attacks[side][BISHOP] = bishopMoves;
	this->attacks[side][ALL] |= this->attacks[side][BISHOP]; 
	
}

void PieceManager::generateKnightMoves(bool side){
 uint64_t bitboard = this->Pieces[side][KNIGHT]; 
 uint64_t knightMoves = 0ULL;

 while (bitboard != 0ULL){
	int index = countr_zero(bitboard);
	bitboard = bitclear(bitboard, index);

	knightMoves |= knightLookups[index] & ~(this->Pieces[side][ALL]);

	addPossibleMove(index, knightLookups[index] & ~(this->Pieces[side][ALL]));
 }

 this->attacks[side][KNIGHT] = knightMoves;
 this->attacks[side][ALL] |= this->attacks[side][KNIGHT];

}

uint64_t PieceManager::outputRank(uint64_t pos){
	uint64_t rank_location = 0;
	for (int i = 0; i < 8; ++i){
		if (ranks[i] & pos)
			rank_location = rank_location | ranks[i];
	}
	return rank_location;
}

int PieceManager::encodeMove(int from, int to, int flags){
	// TODO: Add move encoding code
	return  ((flags & 0xf)<<12) | ((from & 0x3f)<<6) | (to & 0x3f);
}
void PieceManager::addPossibleMove(int start, uint64_t positions){
	// TODO: Add code for storing generated moves
	while (positions != 0ULL){
		int to = countr_zero(positions);
		
		generatedMoves[move_count] = encodeMove(start, to, 0ULL);	
		move_count += 1;	
		positions = bitclear(positions, to);

	}
}

int PieceManager::getEncodedMove(){
	return move_count;
}

bool PieceManager::canKingSideCastle(bool side){
	if (side == WHITE)
		return (Pieces[side][KING] & bitset(E1)) && (Pieces[side][ROOK] & bitset(H1)) && (1ULL << F1 & ~all_pieces) && (1ULL << G1 & ~all_pieces) && !isAttacked(side, E1) && !isAttacked(side, F1) && !isAttacked(side, G1);
	else
		return 0;
}

bool PieceManager::isAttacked(bool side, int square){
	return this->attacks[!side][ALL] & (1ULL >> square);
}

bool PieceManager::canQueenSideCastle(bool side){
	return false;
}
