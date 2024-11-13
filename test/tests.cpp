#include <gtest/gtest.h>
#include "../src/pieces.h"
#include "../src/types.h"

TEST(KingTests, WhiteKingStartingMoves){
	Pieces p = Pieces();
	EXPECT_EQ(p.generateWhiteKingMoves(), 0);
}

TEST(KingTests, EmptyBoardCenterWhiteKingMoves){
	Pieces p = Pieces();

	PieceArgs args {.w_king_bb = bitset(E4)};
	p.setBoard(args);
	EXPECT_EQ(p.generateWhiteKingMoves(), 120596463616);
}

TEST(KingTests, EmptyBoardCornerWhiteKingMoves){
	Pieces p = Pieces();
	PieceArgs args {.w_king_bb = bitset(A8)};
	p.setBoard(args);
	EXPECT_EQ(p.generateWhiteKingMoves(), 4665729213955833856);
}

TEST(KingTests, MoveToEnemySquare){
	Pieces p = Pieces();
	
	PieceArgs args {.w_king_bb = bitset(E4), .b_pawns_bb = bitset(E5)};
	p.setBoard(args);

	EXPECT_EQ(p.generateWhiteKingMoves(), 120596463616);
}

TEST(KnightTests, WhiteKnightStartingMoves){
	Pieces p = Pieces();
	EXPECT_EQ(p.generateWhiteKnightMoves(), 10813440);
}

TEST(KnightTests, EmptyBoardCenter){
		
	Pieces p = Pieces();	
	PieceArgs args {.w_knights_bb = bitset(D5)};
	p.setBoard(args);
	
	EXPECT_EQ(p.generateWhiteKnightMoves(), 11333767002587136);
}

TEST(KnightTests, EmptyBoardCornerKnightMoves){
	
	Pieces p = Pieces();	
	PieceArgs args {.w_knights_bb = bitset(A8)};
	p.setBoard(args);
	
	EXPECT_EQ(p.generateWhiteKnightMoves(), 9077567998918656);
}

TEST(KnightTests, KnightMoveToEnemySquare){
	
	Pieces p = Pieces();
	
	PieceArgs args {.w_knights_bb = bitset(D5), .b_pawns_bb = bitset(B6)};
	p.setBoard(args);

	EXPECT_EQ(p.generateWhiteKnightMoves(), 11333767002587136);
}

TEST(PawnTests, WhitePawnsStartingMoves){
	Pieces p = Pieces();
	EXPECT_EQ(p.generateWhitePawnMoves(), 4294901760);
}

TEST(PawnTests, BlockedByOpponent){
	Pieces p = Pieces();

	PieceArgs args {.w_pawns_bb = bitset(D5) | bitset(B5), .b_pawns_bb = bitset(D6)};
	p.setBoard(args);
	
	EXPECT_EQ(p.generateWhitePawnMoves(), 18084767253659648);
}

TEST(PawnTests, CaputureEnemyPiece){

	Pieces p = Pieces();

	PieceArgs args {.w_pawns_bb = bitset(C5), .b_pawns_bb = bitset(D6)};
	p.setBoard(args);
	EXPECT_EQ(p.generateWhitePawnMoves(), 9059975812874240);	
}	
