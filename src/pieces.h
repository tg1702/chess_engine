#include <iostream>
#include "types.h"
#include "magic.h"
#include "lookup.h"

using namespace std;

#pragma once
class PieceManager{
	private:
		uint64_t w_king;
		uint64_t w_rooks;
		uint64_t w_pawns;
		uint64_t w_bishops;
		uint64_t w_queens;
		uint64_t w_knights;
		uint64_t b_king; 

		uint64_t b_pawns;
		uint64_t b_queens;
		uint64_t b_bishops;
		uint64_t b_rooks;
		uint64_t b_knights;
		uint64_t b_all_pieces;

		uint64_t w_all_pieces;
		uint64_t all_pieces;
	public:
		PieceManager(){
			
			w_king = W_KING_START;
			w_rooks = W_ROOKS_START;
			w_pawns = W_PAWNS_START;
			w_bishops = W_BISHOPS_START;
			w_queens = W_QUEEN_START;
			w_knights = W_KNIGHTS_START;
			b_king =  B_KING_START; 

			b_pawns = B_PAWNS_START;
			b_queens = B_QUEEN_START;
			b_bishops = B_BISHOPS_START;
			b_rooks = B_ROOKS_START;
			b_knights = B_KNIGHTS_START;
			b_all_pieces = B_ALL_PIECES_START;

			w_all_pieces = W_ALL_PIECES_START;
			all_pieces = b_all_pieces | w_all_pieces;
		}
		void setBoard(PieceArgs args);
		void setAllPieces(uint64_t);
		void setBlackPieces(uint64_t);
		void setWhitePieces(uint64_t);
		void setWhiteKingPos(uint64_t);
		void setWhiteKnightsPos(uint64_t);
		void setWhitePawnsPos(uint64_t);
		void setWhiteRooksPos(uint64_t);
		void setWhiteBishopsPos(uint64_t);
		void setWhiteQueensPos(uint64_t);
		void setBlackKingPos(uint64_t);
		void setBlackKnightsPos(uint64_t);
		void setBlackPawnsPos(uint64_t);
	
		uint64_t getWhiteQueenMoves();	
		uint64_t getWhitePawnMoves();
		uint64_t getWhiteKingMoves();
		uint64_t getWhiteKnightMoves();
		uint64_t getWhiteRookMoves();
		uint64_t getWhiteBishopMoves();
		uint64_t outputRank(uint64_t);
};

void PieceManager::setBoard(PieceArgs args){
	uint64_t w_all_pieces = args.w_king_bb | args.w_knights_bb | args.w_pawns_bb;
	uint64_t b_all_pieces = args.b_king_bb | args.b_knights_bb | args.b_pawns_bb;


	uint64_t all_pieces_bb = w_all_pieces | b_all_pieces;
	setAllPieces(all_pieces_bb);
	setWhitePieces(w_all_pieces);
	setBlackPieces(b_all_pieces);
	
	setWhiteQueensPos(args.w_queens_bb);
	setWhiteBishopsPos(args.w_bishops_bb);
	setWhiteKingPos(args.w_king_bb);
	setWhiteKnightsPos(args.w_knights_bb);
	setWhitePawnsPos(args.w_pawns_bb);
	setBlackKingPos(args.b_king_bb);
	setBlackKnightsPos(args.b_knights_bb);
	setBlackPawnsPos(args.b_pawns_bb);
	setWhiteRooksPos(args.w_rooks_bb);
	setWhiteBishopsPos(args.w_bishops_bb);
	
}

void PieceManager::setAllPieces(uint64_t all_pieces_bb){
	this->all_pieces = all_pieces_bb;
}

void PieceManager::setWhitePieces(uint64_t w_all_pieces_bb){
	this->w_all_pieces = w_all_pieces_bb;
}

void PieceManager::setBlackPieces(uint64_t b_all_pieces_bb){
	this->b_all_pieces = b_all_pieces_bb;
}
void PieceManager::setWhiteKingPos(uint64_t w_king_bb){
	this->w_king = w_king_bb;
}

void PieceManager::setWhiteQueensPos(uint64_t w_queens_bb){
	this->w_queens = w_queens_bb;
}
void PieceManager::setWhiteKnightsPos(uint64_t w_knights_bb){
	this->w_knights = w_knights_bb;
}

void PieceManager::setWhitePawnsPos(uint64_t w_pawns_bb){
	this->w_pawns = w_pawns_bb;
}

void PieceManager::setWhiteBishopsPos(uint64_t w_bishops_bb){
	this->w_bishops = w_bishops_bb;
}

void PieceManager::setBlackKingPos(uint64_t b_king_bb){
	this->b_king = b_king_bb;
}

void PieceManager::setBlackKnightsPos(uint64_t b_knights_bb){
	this->b_knights = b_knights_bb;
}

void PieceManager::setBlackPawnsPos(uint64_t b_pawns_bb){
	this->b_pawns = b_pawns_bb;
}

void PieceManager::setWhiteRooksPos(uint64_t w_rooks_bb){
	this->w_rooks = w_rooks_bb;
}

uint64_t PieceManager::getWhitePawnMoves(){
	uint64_t oneSquareMoves = w_pawns << 8;
	uint64_t twoSquareMoves = w_pawns << 16;

	uint64_t captureMoves = (w_pawns << 7 | w_pawns << 9) & b_all_pieces & outputRank(oneSquareMoves);

	return (oneSquareMoves & ~all_pieces) | (twoSquareMoves & ~(all_pieces<<8)) | captureMoves;
}

uint64_t PieceManager::getWhiteKingMoves(){
	uint64_t bitboard = w_king;
 	uint64_t kingMoves = 0ULL;
 	
	while (bitboard != 0ULL){
		int index = countr_zero(bitboard);
		bitboard = bitclear(bitboard, index);

		kingMoves |= whiteKingLookups[index];
 	}


 	return kingMoves & ~w_all_pieces;
 
}

uint64_t PieceManager::getWhiteRookMoves(){
	uint64_t bitboard = w_rooks;
 	uint64_t rookMoves = 0ULL;

	while (bitboard != 0ULL){
		int square = countr_zero(bitboard);

		
		int index = generateMagicIndex((w_all_pieces | b_all_pieces) & rookOccupancyMasks[square], rookMagics[square], square, 0);
	
		rookMoves |= rookMoveList[square][index];

		bitboard = bitclear(bitboard, square);

 	}

	return rookMoves & ~w_all_pieces;
	
}


uint64_t PieceManager::getWhiteQueenMoves(){
	uint64_t bitboard = w_queens;
 	uint64_t rookMoves = 0ULL;
	uint64_t bishopMoves = 0ULL;

	while (bitboard != 0ULL){
		int square = countr_zero(bitboard);

		
		int rookIndex = generateMagicIndex((w_all_pieces | b_all_pieces) & rookOccupancyMasks[square], rookMagics[square], square, 0);
	
		rookMoves |= rookMoveList[square][rookIndex];
		int bishopIndex = generateMagicIndex((w_all_pieces | b_all_pieces) & bishopOccupancyMasks[square], bishopMagics[square], square, 1);

		bishopMoves |= bishopMoveList[square][rookIndex];
		bitboard = bitclear(bitboard, square);

 	}

	return (bishopMoves | rookMoves) & ~w_all_pieces;
	
}
uint64_t PieceManager::getWhiteBishopMoves(){
	uint64_t bitboard = w_bishops;
 	uint64_t bishopMoves = 0ULL;
 	
	while (bitboard != 0ULL){
	
		int square = countr_zero(bitboard);

		
		int index = generateMagicIndex((w_all_pieces | b_all_pieces) & bishopOccupancyMasks[square], bishopMagics[square], square, 1);
	
		bishopMoves |= bishopMoveList[square][index];

		bitboard = bitclear(bitboard, square);

 	}

	return bishopMoves & ~w_all_pieces;
	
}

uint64_t PieceManager::getWhiteKnightMoves(){
 
 uint64_t twoRowAheadLocation = outputRank(w_knights << 16);
 uint64_t oneRowAheadLocation = outputRank(w_knights << 8);

 uint64_t twoRowBehindLocation = outputRank(w_knights >> 16);
 uint64_t oneRowBehindLocation = outputRank(w_knights >> 8);
 uint64_t forwardKnightMoves = (((w_knights << 15) | (w_knights << 17)) & twoRowAheadLocation) | (((w_knights << 6) | (w_knights << 10)) & oneRowAheadLocation); 
 uint64_t backwardKnightMoves = (((w_knights >> 15) | (w_knights >> 17)) & twoRowBehindLocation) | (((w_knights >> 6) | (w_knights >> 10)) & oneRowBehindLocation);

 
 uint64_t knightMoves = forwardKnightMoves | backwardKnightMoves;

 return  knightMoves & ~w_all_pieces;
}

uint64_t PieceManager::outputRank(uint64_t pos){
	uint64_t rank_location = 0;
	for (int i = 0; i < 8; ++i){
		if (ranks[i] & pos)
			rank_location = rank_location | ranks[i];
	}
	return rank_location;
}

