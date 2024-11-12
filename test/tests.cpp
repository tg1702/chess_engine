#include <gtest/gtest.h>
#include "../src/pieces.h"

#define bitset(byte, nbit) (byte = 0ULL | (1ULL<< (nbit)))


TEST(KingTests, WhiteKingStartingMoves){
	Pieces p = Pieces();
	EXPECT_EQ(p.generateWhiteKingMoves(), 0);
}

TEST(KingTests, EmptyBoardCenterWhiteKingMoves){
	Pieces p = Pieces();

		
	uint64_t empty = 0ULL;
	uint64_t E4 = bitset(empty, 27ULL);
	PieceArgs args {.w_king_bb = E4};
	p.setBoard(args);
	EXPECT_EQ(p.generateWhiteKingMoves(), 120596463616);
}

TEST(KingTests, EmptyBoardCornerWhiteKingMoves){
	Pieces p = Pieces();
	PieceArgs args {.w_king_bb = A8};
	p.setBoard(args);
	EXPECT_EQ(p.generateWhiteKingMoves(), 4665729213955833856);
}

TEST(KingTests, MoveToEnemySquare){
	Pieces p = Pieces();
	
	uint64_t empty = 0ULL;
	
	uint64_t E4 = bitset(empty, 27ULL);
	empty = 0ULL;
	uint64_t E5 = bitset(empty, 35ULL);
	PieceArgs args {.w_king_bb = E4, .b_pawns_bb = E5};
	p.setBoard(args);

	EXPECT_EQ(p.generateWhiteKingMoves(), 120596463616);
}

TEST(KnightTests, WhiteKnightStartingMoves){
	Pieces p = Pieces();
	EXPECT_EQ(p.generateWhiteKnightMoves(), 10813440);
}

TEST(KnightTests, EmptyBoardCenter){
		
	uint64_t empty = 0ULL;
	uint64_t D5 = bitset(empty, 36ULL);
	Pieces p = Pieces();	
	PieceArgs args {.w_knights_bb = D5};
	p.setBoard(args);
	
	EXPECT_EQ(p.generateWhiteKnightMoves(), 11333767002587136);
}

TEST(KnightTests, EmptyBoardCornerKnightMoves){
	
	uint64_t empty = 0ULL;
	uint64_t A8 = bitset(empty, 63ULL);
	Pieces p = Pieces();	
	PieceArgs args {.w_knights_bb = A8};
	p.setBoard(args);
	
	EXPECT_EQ(p.generateWhiteKnightMoves(), 9077567998918656);
}

TEST(KnightTests, KnightMoveToEnemySquare){
	
	Pieces p = Pieces();
	
	uint64_t empty = 0ULL;
	
	uint64_t D5 = bitset(empty, 36ULL);
	empty = 0ULL;
	uint64_t B6 = bitset(empty, 46ULL);
	PieceArgs args {.w_knights_bb = D5, .b_pawns_bb = B6};
	p.setBoard(args);

	EXPECT_EQ(p.generateWhiteKnightMoves(), 11333767002587136);
}

TEST(PawnTests, WhitePawnsStartingMoves){
	Pieces p = Pieces();
	EXPECT_EQ(p.generateWhitePawnMoves(), 4294901760);
}

TEST(PawnTests, BlockedByOpponent){
	Pieces p = Pieces();

	uint64_t empty = 0ULL;

	
	uint64_t D5 = bitset(empty, 36ULL);
	empty = 0ULL;
	uint64_t D6 = bitset(empty, 44ULL);
	empty = 0ULL;
	uint64_t B5 = bitset(empty, 38ULL);
	PieceArgs args {.w_pawns_bb = D5 | B5, .b_pawns_bb = D6};
	p.setBoard(args);
	
	EXPECT_EQ(p.generateWhitePawnMoves(), 18084767253659648);
}

TEST(PawnTests, CaputureEnemyPiece){

	Pieces p = Pieces();

	uint64_t empty = 0ULL;

	
	uint64_t D5 = bitset(empty, 36ULL);
	empty = 0ULL;
	uint64_t D6 = bitset(empty, 44ULL);
	empty = 0ULL;
	uint64_t C5 = bitset(empty, 37ULL);

	PieceArgs args {.w_pawns_bb = C5, .b_pawns_bb = D6};
	p.setBoard(args);
	EXPECT_EQ(p.generateWhitePawnMoves(), 9059975812874240);	
}	
