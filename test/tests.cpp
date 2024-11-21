#include <gtest/gtest.h>
#include "../src/pieces.h"
#include "../src/types.h"

TEST(KingTests, WhiteKingStartingMoves){
	PieceManager p = PieceManager();
	EXPECT_EQ(p.getWhiteKingMoves(), 0);
}

TEST(KingTests, EmptyBoardCenterWhiteKingMoves){
	PieceManager p = PieceManager();

	PieceArgs args {.w_king_bb = bitset(E4)};
	p.setBoard(args);
	EXPECT_EQ(p.getWhiteKingMoves(), 120596463616);
}

TEST(KingTests, EmptyBoardCornerWhiteKingMoves){
	PieceManager p = PieceManager();
	PieceArgs args {.w_king_bb = bitset(A8)};
	p.setBoard(args);
	EXPECT_EQ(p.getWhiteKingMoves(), 4665729213955833856);
}

TEST(KingTests, MoveToEnemySquare){
	PieceManager p = PieceManager();
	
	PieceArgs args {.w_king_bb = bitset(E4), .b_pawns_bb = bitset(E5)};
	p.setBoard(args);

	EXPECT_EQ(p.getWhiteKingMoves(), 120596463616);
}

TEST(KnightTests, WhiteKnightStartingMoves){
	PieceManager p = PieceManager();
	EXPECT_EQ(p.getWhiteKnightMoves(), 10813440);
}

TEST(KnightTests, EmptyBoardCenter){
		
	PieceManager p = PieceManager();	
	PieceArgs args {.w_knights_bb = bitset(D5)};
	p.setBoard(args);
	
	EXPECT_EQ(p.getWhiteKnightMoves(), 11333767002587136);
}

TEST(KnightTests, EmptyBoardCornerKnightMoves){
	
	PieceManager p = PieceManager();	
	PieceArgs args {.w_knights_bb = bitset(A8)};
	p.setBoard(args);
	
	EXPECT_EQ(p.getWhiteKnightMoves(), 9077567998918656);
}

TEST(KnightTests, KnightMoveToEnemySquare){
	
	PieceManager p = PieceManager();
	
	PieceArgs args {.w_knights_bb = bitset(D5), .b_pawns_bb = bitset(B6)};
	p.setBoard(args);

	EXPECT_EQ(p.getWhiteKnightMoves(), 11333767002587136);
}

TEST(PawnTests, WhitePawnsStartingMoves){
	PieceManager p = PieceManager();
	EXPECT_EQ(p.getWhitePawnMoves(), 4294901760);
}

TEST(PawnTests, BlockedByOpponent){
	PieceManager p = PieceManager();

	PieceArgs args {.w_pawns_bb = bitset(D4), .b_pawns_bb = bitset(D5)};
	p.setBoard(args);
	
	EXPECT_EQ(p.getWhitePawnMoves(), 0);
}

TEST(PawnTests, CaputureEnemyPiece){

	PieceManager p = PieceManager();

	PieceArgs args {.w_pawns_bb = bitset(C5), .b_pawns_bb = bitset(D6)};
	p.setBoard(args);
	EXPECT_EQ(p.getWhitePawnMoves(), 9059975812874240);	
}

TEST(PawnTests, EdgeFileCaptureTest){
	PieceManager p = PieceManager();

	PieceArgs args {.w_pawns_bb = bitset(A5), .b_pawns_bb = bitset(H6)};
	p.setBoard(args);
	EXPECT_EQ(p.getWhitePawnMoves(), bitset(A6) | bitset(A7));

}

TEST(RookTests, CenterWithBlockers){
	PieceManager p = PieceManager();

	
	PieceArgs args {.w_rooks_bb = bitset(D4), .w_pawns_bb = bitset(C4), .b_pawns_bb = bitset(D6)};
	p.setBoard(args);
	EXPECT_EQ(p.getWhiteRookMoves(), bitset(E4) | bitset(F4) | bitset(G4) | bitset(H4) | bitset(D1) | bitset(D2) | bitset(D3) | bitset(D5) | bitset(D6));

	// White behind black
	
	PieceManager p2 = PieceManager();

	
	PieceArgs args2 {.w_rooks_bb = bitset(D4), .w_pawns_bb = bitset(A4), .w_knights_bb = bitset(D6), .b_pawns_bb = bitset(B4) | bitset(D7)};
	p2.setBoard(args2);

	EXPECT_EQ(p2.getWhiteRookMoves(), bitset(C4) | bitset(B4) | bitset(E4) | bitset(F4) | bitset(G4) | bitset(H4) | bitset(D1) | bitset(D2) | bitset(D3) | bitset(D5)); 
}


TEST(RookTests, CornerWithBlockers){
	PieceManager p = PieceManager();

	
	PieceArgs args {.w_king_bb = bitset(B7), .w_rooks_bb = bitset(A8), .w_pawns_bb = bitset(C4) | bitset(D4), .b_knights_bb = bitset(D8) | bitset(A4)};
	p.setBoard(args);
	EXPECT_EQ(p.getWhiteRookMoves(), bitset(A7) | bitset(A6) | bitset(A5) | bitset(A4) | bitset(B8) | bitset(C8) | bitset(D8));

	// White behind black
 
}

TEST(RookTests, StartingPosition){

	PieceManager p = PieceManager();

	EXPECT_EQ(p.getWhiteRookMoves(), 0);
}
