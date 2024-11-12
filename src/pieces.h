#include <iostream>

using namespace std;

enum StartingPositions{
	W_KING_START = 0b0000000000000000000000000000000000000000000000000000000000001000,
	W_ROOKS_START = 0b0000000000000000000000000000000000000000000000000000000010000001,
	W_PAWNS_START = 0b0000000000000000000000000000000000000000000000001111111100000000,
        W_BISHOPS_START = 0b0000000000000000000000000000000000000000000000000000000000100100,
	W_QUEEN_START = 0b0000000000000000000000000000000000000000000000000000000000010000,
	W_KNIGHTS_START = 0b0000000000000000000000000000000000000000000000000000000001000010,

	B_KING_START = 0b0000100000000000000000000000000000000000000000000000000000000000,
	B_PAWNS_START = 0b0000000011111111000000000000000000000000000000000000000000000000,
	B_QUEEN_START = 0b0001000000000000000000000000000000000000000000000000000000001000,
	B_BISHOPS_START = 0b0010010000000000000000000000000000000000000000000000000000001000,
	B_ROOKS_START = 0b100000100000000000000000000000000000000000000000000000000001000,
	B_KNIGHTS_START = 0b0100001000000000000000000000000000000000000000000000000000001000,

	B_ALL_PIECES_START = 0b1111111111111111000000000000000000000000000000000000000000000000,
	W_ALL_PIECES_START = 0b0000000000000000000000000000000000000000000000001111111111111111,

	RANK_1 = 0b0000000000000000000000000000000000000000000000000000000011111111,
	RANK_2 = 0b0000000000000000000000000000000000000000000000001111111100000000,
	RANK_3 = 0b0000000000000000000000000000000000000000111111110000000000000000,	
	RANK_4 = 0b0000000000000000000000000000000011111111000000000000000000000000,
	RANK_5 = 0b0000000000000000000000001111111100000000000000000000000000000000,
	RANK_6 = 0b0000000000000000111111110000000000000000000000000000000000000000,
	RANK_7 = 0b0000000011111111000000000000000000000000000000000000000000000000,
	RANK_8 = 0b1111111100000000000000000000000000000000000000000000000000000000,
};
uint64_t ranks[] = {RANK_1, RANK_2, RANK_3, RANK_4,RANK_5, RANK_6, RANK_7, RANK_8};

#pragma once


struct PieceArgs{
	
		uint64_t w_king_bb = 0;
		uint64_t w_rooks_bb = 0;
		uint64_t w_pawns_bb = 0;
		uint64_t w_bishops_bb = 0;
		uint64_t w_queen_bb = 0;
		uint64_t w_knights_bb = 0;
		uint64_t b_king_bb = 0; 

		uint64_t b_pawns_bb = 0;
		uint64_t b_queen_bb = 0;
		uint64_t b_bishops_bb = 0;
		uint64_t b_rooks_bb = 0;
		uint64_t b_knights_bb = 0;
		uint64_t b_all_pieces_bb = 0;

		uint64_t w_all_pieces_bb = 0;
		uint64_t all_pieces_bb = 0;
			
};

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
	setAllPieces(args.all_pieces_bb);
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

	//cout << "binary " << oneSquareMoves | twoSquareMoves | captureMoves << endl;

	return oneSquareMoves | twoSquareMoves | captureMoves;	
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

