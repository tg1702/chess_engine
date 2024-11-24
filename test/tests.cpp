#include <gtest/gtest.h>
#include "../src/pieces.h"
#include "../src/types.h"

TEST(KingTests, WhiteKingStartingMoves){
	PieceManager p = PieceManager();
	EXPECT_EQ(p.getKingMoves(WHITE), 0);
}

TEST(KingTests, EmptyBoardCenterWhiteKingMoves){
	PieceManager p = PieceManager();

	PieceArgs args {.w_king_bb = bitset(E4)};
	p.setBoard(args);
	EXPECT_EQ(p.getKingMoves(WHITE), 120596463616);
}

TEST(KingTests, EmptyBoardCornerWhiteKingMoves){
	PieceManager p = PieceManager();
	PieceArgs args {.w_king_bb = bitset(A8)};
	p.setBoard(args);
	EXPECT_EQ(p.getKingMoves(WHITE), 4665729213955833856);
}

TEST(KingTests, MoveToEnemySquare){
	PieceManager p = PieceManager();
	
	PieceArgs args {.w_king_bb = bitset(E4), .b_pawns_bb = bitset(E5)};
	p.setBoard(args);

	EXPECT_EQ(p.getKingMoves(WHITE), 120596463616);
}

TEST(KnightTests, WhiteKnightStartingMoves){
	PieceManager p = PieceManager();
	EXPECT_EQ(p.getKnightMoves(WHITE), 10813440);
}

TEST(KnightTests, EmptyBoardCenter){
		
	PieceManager p = PieceManager();	
	PieceArgs args {.w_knights_bb = bitset(D5)};
	p.setBoard(args);
	
	EXPECT_EQ(p.getKnightMoves(WHITE), 11333767002587136);
}

TEST(KnightTests, EmptyBoardCornerKnightMoves){
	
	PieceManager p = PieceManager();	
	PieceArgs args {.w_knights_bb = bitset(A8)};
	p.setBoard(args);
	
	EXPECT_EQ(p.getKnightMoves(WHITE), 9077567998918656);
}

TEST(KnightTests, KnightMoveToEnemySquare){
	
	PieceManager p = PieceManager();
	
	PieceArgs args {.w_knights_bb = bitset(D5), .b_pawns_bb = bitset(B6)};
	p.setBoard(args);

	EXPECT_EQ(p.getKnightMoves(WHITE), 11333767002587136);
}

TEST(PawnTests, WhitePawnsStartingMoves){
	PieceManager p = PieceManager();
	EXPECT_EQ(p.getPawnMoves(WHITE), 4294901760);
}

TEST(PawnTests, BlockedByOpponent){
	PieceManager p = PieceManager();

	PieceArgs args {.w_pawns_bb = bitset(D4), .b_pawns_bb = bitset(D5)};
	p.setBoard(args);
	
	EXPECT_EQ(p.getPawnMoves(WHITE), 0);
}

TEST(PawnTests, CaputureEnemyPiece){

	PieceManager p = PieceManager();

	PieceArgs args {.w_pawns_bb = bitset(C5), .b_pawns_bb = bitset(D6)};
	p.setBoard(args);
	EXPECT_EQ(p.getPawnMoves(WHITE), 0x300000000000);	
}

TEST(PawnTests, EdgeFileCaptureTest){
	PieceManager p = PieceManager();

	PieceArgs args {.w_pawns_bb = bitset(A5), .b_pawns_bb = bitset(H6)};
	p.setBoard(args);
	EXPECT_EQ(p.getPawnMoves(WHITE), bitset(A6));

}

TEST(RookTests, CenterWithBlockers){
	PieceManager p = PieceManager();

	
	PieceArgs args {.w_rooks_bb = bitset(D4), .w_pawns_bb = bitset(C4), .b_pawns_bb = bitset(D6)};
	p.setBoard(args);
	EXPECT_EQ(p.getRookMoves(WHITE), bitset(E4) | bitset(F4) | bitset(G4) | bitset(H4) | bitset(D1) | bitset(D2) | bitset(D3) | bitset(D5) | bitset(D6));

	// White behind black
	
	PieceManager p2 = PieceManager();

	
	PieceArgs args2 {.w_rooks_bb = bitset(D4), .w_pawns_bb = bitset(A4), .w_knights_bb = bitset(D6), .b_pawns_bb = bitset(B4) | bitset(D7)};
	p2.setBoard(args2);

	EXPECT_EQ(p2.getRookMoves(WHITE), bitset(C4) | bitset(B4) | bitset(E4) | bitset(F4) | bitset(G4) | bitset(H4) | bitset(D1) | bitset(D2) | bitset(D3) | bitset(D5)); 
}


TEST(RookTests, A8CornerWithBlockers){
	PieceManager p = PieceManager();

	
	PieceArgs args {.w_king_bb = bitset(B7), .w_rooks_bb = bitset(A8), .w_pawns_bb = bitset(C4) | bitset(D4), .b_knights_bb = bitset(D8) | bitset(A4)};
	p.setBoard(args);
	EXPECT_EQ(p.getRookMoves(WHITE), bitset(A7) | bitset(A6) | bitset(A5) | bitset(A4) | bitset(B8) | bitset(C8) | bitset(D8));

	// White behind black
 
}


TEST(RookTests, H1CornerWithBlockers){
	PieceManager p = PieceManager();

	
	PieceArgs args {.w_king_bb = bitset(G2), .w_rooks_bb = bitset(H1), .w_pawns_bb = bitset(C4) | bitset(D1), .b_knights_bb = bitset(H8)};
	p.setBoard(args);
	EXPECT_EQ(p.getRookMoves(WHITE), 72340172838076686);

	// White behind black
 
}
TEST(RookTests, StartingPosition){

	PieceManager p = PieceManager();

	EXPECT_EQ(p.getRookMoves(WHITE), 0);
}

TEST(BishopTests, CenterWithBlockers){
	PieceManager p = PieceManager();

	PieceArgs args {.w_pawns_bb = bitset(F6), .w_bishops_bb = bitset(D4), .b_pawns_bb = bitset(B2)};
	p.setBoard(args);
	EXPECT_EQ(p.getBishopMoves(WHITE),0x80402800284402);


	PieceManager p2 = PieceManager();

	PieceArgs args2 {.w_pawns_bb = bitset(B6), .w_bishops_bb = bitset(D4), .b_pawns_bb = bitset(G1)};
	p2.setBoard(args2);
	EXPECT_EQ(p2.getBishopMoves(WHITE),0x102042800284482);
}

TEST(BishopTests, H1CornerWithBlockers){
	PieceManager p = PieceManager();

	
	PieceArgs args {.w_king_bb = bitset(B7), .w_bishops_bb = bitset(H1), .b_knights_bb = bitset(C6)};
	p.setBoard(args);
	EXPECT_EQ(p.getBishopMoves(WHITE), 0x201008040200);

	// White behind black
 
}

TEST(BishopTests, A8CornerWithBlockers){
	PieceManager p = PieceManager();

	
	PieceArgs args {.w_bishops_bb = bitset(A8), .b_knights_bb = bitset(H1)};
	p.setBoard(args);
	EXPECT_EQ(p.getBishopMoves(0), 0x40201008040201);

	// White behind black
 
}

TEST(BishopTests, StartingPosition){
	PieceManager p = PieceManager();

	EXPECT_EQ(p.getBishopMoves(0), 0);

}

TEST(QueenTests, EmptyBoardCenter){
	PieceManager p = PieceManager();
	PieceArgs args {.w_queens_bb = bitset(D4)};
	p.setBoard(args);
	EXPECT_EQ(p.getQueenMoves(0), 1266167048752878738);


}

TEST(QueenTests, StartingPosition){
	PieceManager p = PieceManager();
	EXPECT_EQ(p.getQueenMoves(0), 0);

}

TEST(MakeMoveTests, StartingPosition){
	PieceManager p = PieceManager();
	p.makeMove(E2, E4, WHITE);
	EXPECT_EQ(p.getPawnMoves(WHITE), 38519898112);
}
