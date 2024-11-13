#include <iostream>
#include "types.h"

using namespace std;

#pragma once
class Pieces{
	private:
		uint64_t w_king;
		uint64_t w_rooks;
		uint64_t w_pawns;
		uint64_t w_bishops;
		uint64_t w_queen;
		uint64_t w_knights;
		uint64_t b_king; 

		uint64_t b_pawns;
		uint64_t b_queen;
		uint64_t b_bishops;
		uint64_t b_rooks;
		uint64_t b_knights;
		uint64_t b_all_pieces;

		uint64_t w_all_pieces;
		uint64_t all_pieces;
	public:
		Pieces(){
			
			w_king = W_KING_START;
			w_rooks = W_ROOKS_START;
			w_pawns = W_PAWNS_START;
			w_bishops = W_BISHOPS_START;
			w_queen = W_QUEEN_START;
			w_knights = W_KNIGHTS_START;
			b_king =  B_KING_START; 

			b_pawns = B_PAWNS_START;
			b_queen = B_QUEEN_START;
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
		
		void setBlackKingPos(uint64_t);
		void setBlackKnightsPos(uint64_t);
		void setBlackPawnsPos(uint64_t);
		uint64_t generateWhitePawnMoves();
		uint64_t generateWhiteKingMoves();
		uint64_t generateWhiteKnightMoves();
		uint64_t outputRank(uint64_t);
};

void Pieces::setBoard(PieceArgs args){
	uint64_t w_all_pieces = args.w_king_bb | args.w_knights_bb | args.w_pawns_bb;
	uint64_t b_all_pieces = args.b_king_bb | args.b_knights_bb | args.b_pawns_bb;


	uint64_t all_pieces_bb = w_all_pieces | b_all_pieces;
	setAllPieces(all_pieces_bb);
	setWhitePieces(w_all_pieces);
	setBlackPieces(b_all_pieces);

	setWhiteKingPos(args.w_king_bb);
	setWhiteKnightsPos(args.w_knights_bb);
	setWhitePawnsPos(args.w_pawns_bb);
	setBlackKingPos(args.b_king_bb);
	setBlackKnightsPos(args.b_knights_bb);
	setBlackPawnsPos(args.b_pawns_bb);
	
}

void Pieces::setAllPieces(uint64_t all_pieces_bb){
	this->all_pieces = all_pieces_bb;
}

void Pieces::setWhitePieces(uint64_t w_all_pieces_bb){
	this->w_all_pieces = w_all_pieces_bb;
}

void Pieces::setBlackPieces(uint64_t b_all_pieces_bb){
	this->b_all_pieces = b_all_pieces_bb;
}
void Pieces::setWhiteKingPos(uint64_t w_king_bb){
	this->w_king = w_king_bb;
}

void Pieces::setWhiteKnightsPos(uint64_t w_knights_bb){
	this->w_knights = w_knights_bb;
}

void Pieces::setWhitePawnsPos(uint64_t w_pawns_bb){
	this->w_pawns = w_pawns_bb;
}

void Pieces::setBlackKingPos(uint64_t b_king_bb){
	this->b_king = b_king_bb;
}

void Pieces::setBlackKnightsPos(uint64_t b_knights_bb){
	this->b_knights = b_knights_bb;
}

void Pieces::setBlackPawnsPos(uint64_t b_pawns_bb){
	this->b_pawns = b_pawns_bb;
}
uint64_t Pieces::generateWhitePawnMoves(){
	uint64_t oneSquareMoves = w_pawns << 8;
	uint64_t twoSquareMoves = w_pawns << 16;

	uint64_t captureMoves = (w_pawns << 7 | w_pawns << 9) & b_all_pieces;


	return (oneSquareMoves & ~all_pieces | (((oneSquareMoves & ~all_pieces) << 8 ) &~all_pieces) ) | captureMoves;	
}

uint64_t Pieces::generateWhiteKingMoves(){
	uint64_t oneRowAhead = outputRank(w_king << 8);
	uint64_t sameRow = outputRank(w_king);
	uint64_t oneRowBehind = outputRank(w_king >> 8);

	uint64_t kingMoves = (((w_king << 8) | (w_king << 7) | (w_king << 9)) & oneRowAhead) | (((w_king << 1) | (w_king >> 1)) & sameRow) | (((w_king >> 8) | (w_king >> 7) | (w_king >> 9)) & oneRowBehind);
       
	return kingMoves & ~w_all_pieces;	
}

uint64_t Pieces::generateWhiteKnightMoves(){
 uint64_t twoRowAheadLocation = outputRank(w_knights << 16);
 uint64_t oneRowAheadLocation = outputRank(w_knights << 8);

 uint64_t twoRowBehindLocation = outputRank(w_knights >> 16);
 uint64_t oneRowBehindLocation = outputRank(w_knights >> 8);
 uint64_t forwardKnightMoves = (((w_knights << 15) | (w_knights << 17)) & twoRowAheadLocation) | (((w_knights << 6) | (w_knights << 10)) & oneRowAheadLocation); 
 uint64_t backwardKnightMoves = (((w_knights >> 15) | (w_knights >> 17)) & twoRowBehindLocation) | (((w_knights >> 6) | (w_knights >> 10)) & oneRowBehindLocation);

 return (forwardKnightMoves | backwardKnightMoves) & ~w_all_pieces;
}

uint64_t Pieces::outputRank(uint64_t pos){
	uint64_t rank_location = 0;
	for (int i = 0; i < 8; ++i){
		if (ranks[i] & pos)
			rank_location = rank_location | ranks[i];
	}
	return rank_location;
}

