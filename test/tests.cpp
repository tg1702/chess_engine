#include <gtest/gtest.h>
#include "../src/pieces.h"

#define bitset(byte, nbit) (byte = 0ULL | (1ULL<< (nbit)))


uint64_t A8 = 0b1000000000000000000000000000000000000000000000000000000000000000;
// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

TEST(KingTests, WhiteKingStartingMoves){
	Pieces p = Pieces();
	EXPECT_EQ(p.generateWhiteKingMoves(), 0);
}

TEST(KingTests, EmptyBoardCenterWhiteKingMoves){
	Pieces p = Pieces();

		
	uint64_t empty = 0ULL;
	uint64_t E4 = bitset(empty, 27ULL);
	PieceArgs args {.w_king_bb = E4, .all_pieces_bb = E4};
	p.setBoard(args);
	EXPECT_EQ(p.generateWhiteKingMoves(), 120596463616);
}

TEST(KingTests, EmptyBoardCornerWhiteKingMoves){
	Pieces p = Pieces();
	PieceArgs args {.w_king_bb = A8, .all_pieces_bb = A8};
	p.setBoard(args);
	EXPECT_EQ(p.generateWhiteKingMoves(), 4665729213955833856);
}

TEST(KingTests, MoveToEnemySquare){
	Pieces p = Pieces();
	
	uint64_t empty = 0ULL;
	
	uint64_t E4 = bitset(empty, 27ULL);
	empty = 0ULL;
	uint64_t E5 = bitset(empty, 35ULL);
	PieceArgs args {.w_king_bb = E4, .b_pawns_bb = E5, .all_pieces_bb = E4 | E5};
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
	PieceArgs args {.w_knights_bb = D5, .all_pieces_bb = D5};
	p.setBoard(args);
	
	//EXPECT_EQ(D4, 268435456);
	EXPECT_EQ(p.generateWhiteKnightMoves(), 11333767002587136);
}

TEST(KnightTests, EmptyBoardCornerKnightMoves){
	
	uint64_t empty = 0ULL;
	uint64_t A8 = bitset(empty, 63ULL);
	Pieces p = Pieces();	
	PieceArgs args {.w_knights_bb = A8, .all_pieces_bb = A8};
	p.setBoard(args);
	
	EXPECT_EQ(p.generateWhiteKnightMoves(), 9077567998918656);
}	
