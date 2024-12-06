#include <iostream>
#include <gtest/gtest.h>
#include <array>
#ifndef PIECES_H
#define PIECES_H
#endif

#ifndef TYPES_H
#define TYPES_H
#endif

#include "../src/board.h"

TEST(MakeMoveTests, InitialBoardMoves){
	Board board = Board();
	
	std::array <unsigned int, 256> moveTests = { 80, 82, 405, 407, 528, 536, 593, 601, 658, 666, 723, 731, 788, 796, 853, 861, 918, 926, 983, 991, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 
	
	EXPECT_EQ(board.getMove(), moveTests);
	EXPECT_EQ(256 - std::count(moveTests.begin(), moveTests.end(), 0), 20);

}

TEST(Captures, NormalCaptures){
	PieceArgs args {.w_knights_bb = bitset(F3), .b_pawns_bb = bitset(E5)};

	Board board = Board(args);

	EXPECT_EQ(board.makeMove(F3, E5, WHITE), "Piece type = 5 from = 18 to = 35"); 
}

TEST(Captures, PawnCaptures){
	PieceArgs args {.w_pawns_bb = bitset(E4), .b_rooks_bb = bitset(D5)};
	Board board = Board(args);

	board.printBoard();
	EXPECT_EQ(board.makeMove(E4, D5, WHITE), "Piece type = 2 from = 27 to = 36");
	board.printBoard();

}
TEST(CastleTests, WhiteKingSideCastle){
	Board board = Board();
		
	board.makeMove(E2, E4, WHITE);
	board.makeMove(E7, E5, BLACK);
	board.makeMove(F1, C4, WHITE);
	board.makeMove(G8, F6, BLACK);
	board.makeMove(G1, F3, WHITE);
	board.makeMove(B8, C6, BLACK);	
	//board.makeMove(E1, G1, WHITE);	
	EXPECT_EQ(board.makeMove(E1, G1, WHITE), "Piece type = 0 from = 3 to = 1");

}

TEST(CastleTests, InvalidKSCastleMovedWhiteKing){
	Board board = Board();

	
	board.makeMove(E2, E4, WHITE);
	board.makeMove(E7, E5, BLACK);
	board.makeMove(F1, C4, WHITE);
	board.makeMove(G8, F6, BLACK);
	board.makeMove(G1, F3, WHITE);
	board.makeMove(B8, C6, BLACK);
	board.makeMove(E1, E2, WHITE);
	board.makeMove(B8, C6, BLACK);
	board.makeMove(E2, E1, WHITE);
	board.makeMove(A7, A5, BLACK);

	board.printBoard();	
	EXPECT_EQ(board.makeMove(E1, G1, WHITE), "Invalid move 31");	
}

TEST(CastleTests, BishopAttacksG1PreventsKSCastle){
        Board board = Board();


        board.makeMove(E2, E4, WHITE);
        board.makeMove(E7, E5, BLACK);
        board.makeMove(F1, C4, WHITE);
        board.makeMove(G8, F6, BLACK);
        board.makeMove(F2, F4, WHITE);
        board.makeMove(F8, C5, BLACK);
        board.makeMove(G1, F3, WHITE);
        board.makeMove(A7, A5, BLACK);

        board.printBoard();
        EXPECT_EQ(board.makeMove(E1, G1, WHITE), "Invalid move 31");
}


TEST(CastleTests, InvalidKSCastleMovedH1Rook){
	Board board = Board();

	
	board.makeMove(E2, E4, WHITE);
	board.makeMove(E7, E5, BLACK);
	board.makeMove(F1, C4, WHITE);
	board.makeMove(G8, F6, BLACK);
	board.makeMove(G1, F3, WHITE);
	board.makeMove(B8, C6, BLACK);
	board.makeMove(H1, G1, WHITE);
	board.makeMove(B8, C6, BLACK);
	board.makeMove(G1, H1, WHITE);
	board.makeMove(A7, A5, BLACK);

	board.printBoard();	
	EXPECT_EQ(board.makeMove(E1, G1, WHITE), "Invalid move 31");	
}

TEST(CastleTests, WhiteQueensideCastle){
	Board board = Board();

	board.makeMove(E2, E4, WHITE);
	board.makeMove(E7, E5, BLACK);
	board.makeMove(B1, C3, WHITE);
	board.makeMove(B8, C6, BLACK);
	board.makeMove(D2, D3, WHITE);
	board.makeMove(F8, C5, BLACK);
	board.makeMove(C1, E3, WHITE);
	board.makeMove(G8, F6, BLACK);
	board.makeMove(D1, D2, WHITE);
	board.makeMove(A7, A5, BLACK);

	board.printBoard();
	EXPECT_EQ(board.makeMove(E1, C1, WHITE), "Piece type = 0 from = 3 to = 5");
	board.printBoard();
}


TEST(CastleTests, BishopPreventsWhiteQueensideCastle){
	Board board = Board();

	cout << board.makeMove(E2, E4, WHITE) << endl;
	cout << board.makeMove(E7, E5, BLACK) << endl;
	cout << board.makeMove(B1, C3, WHITE) << endl;
	cout << board.makeMove(B8, C6, BLACK) << endl;
	cout << board.makeMove(D2, D3, WHITE) << endl;
	cout << board.makeMove(F8, E7, BLACK) << endl;
	cout << board.makeMove(D1, F3, WHITE) << endl;
	cout << board.makeMove(D7, D5, BLACK) << endl;
	cout << board.makeMove(C1, H6, WHITE) << endl;
	cout << board.makeMove(E7, G5, BLACK) << endl;

	board.printBoard();
	EXPECT_EQ(board.makeMove(E1, C1, WHITE), "Invalid move 35");
	board.printBoard();
}

TEST(CastleTests, InvalidQSMovedKing){
        Board board = Board();

        board.makeMove(E2, E4, WHITE);
        board.makeMove(E7, E5, BLACK);
        board.makeMove(B1, C3, WHITE);
        board.makeMove(B8, C6, BLACK);
        board.makeMove(D2, D3, WHITE);
        board.makeMove(F1, C5, BLACK);
        board.makeMove(C1, E3, WHITE);
        board.makeMove(G8, F6, BLACK);
        board.makeMove(D1, D2, WHITE);
        board.makeMove(A7, A5, BLACK);
	board.makeMove(E1, D1, WHITE);
	board.makeMove(A5, A4, BLACK);
	board.makeMove(D1, E1, WHITE);
	board.makeMove(F6, G4, BLACK);
        board.printBoard();
        EXPECT_EQ(board.makeMove(E1, C1, WHITE), "Invalid move 35");
        board.printBoard();
}

TEST(CastleTests, InvalidQSMovedA1Rook){
        Board board = Board();

        board.makeMove(E2, E4, WHITE);
        board.makeMove(E7, E5, BLACK);
        board.makeMove(B1, C3, WHITE);
        board.makeMove(B8, C6, BLACK);
        board.makeMove(D2, D3, WHITE);
        board.makeMove(F1, C5, BLACK);
        board.makeMove(C1, E3, WHITE);
        board.makeMove(G8, F6, BLACK);
        board.makeMove(D1, D2, WHITE);
        board.makeMove(A7, A5, BLACK);
        board.makeMove(A1, B1, WHITE);
        board.makeMove(A5, A4, BLACK);
        board.makeMove(B1, A1, WHITE);
        board.makeMove(F6, G4, BLACK);
        board.printBoard();
        EXPECT_EQ(board.makeMove(E1, C1, WHITE), "Invalid move 35");
        board.printBoard();
}

/* BLACK */
TEST(CastleTests, BlackKingSideCastle){
	Board board = Board();
		
	board.makeMove(E2, E4, WHITE);
	board.makeMove(E7, E5, BLACK);
	board.makeMove(F1, C4, WHITE);
	board.makeMove(G8, F6, BLACK);
	board.makeMove(G1, F3, WHITE);
	board.makeMove(F8, C5, BLACK);	
	board.makeMove(E1, G1, WHITE);	
	EXPECT_EQ(board.makeMove(E8, G8, BLACK), "Piece type = 0 from = 59 to = 57");

}

TEST(CastleTests, InvalidKSCastleMovedBlackKing){
	Board board = Board();

	
	board.makeMove(E2, E4, WHITE);
	board.makeMove(E7, E5, BLACK);
	board.makeMove(F1, C4, WHITE);
	board.makeMove(G8, F6, BLACK);
	board.makeMove(G1, F3, WHITE);
	board.makeMove(F8, C5, BLACK);
	board.makeMove(D2, D4, WHITE);
	board.makeMove(E8, F8, BLACK);
	board.makeMove(A2, A4, WHITE);
	board.makeMove(F8, E8, BLACK);
	board.makeMove(H2, H4, WHITE);
	board.printBoard();	
	EXPECT_EQ( board.makeMove(E8, G8, BLACK), "Invalid move 5957");	
}

TEST(CastleTests, BishopAttacksG8PreventsKSCastle){
        Board board = Board();


        board.makeMove(E2, E4, WHITE);
        board.makeMove(E7, E5, BLACK);
        board.makeMove(F1, C4, WHITE);
        board.makeMove(F7, F5, BLACK);
        board.makeMove(D2, D3, WHITE);
        board.makeMove(F8, C5, BLACK);
        board.makeMove(G1, F3, WHITE);
        board.makeMove(G8, F6, BLACK);
	board.makeMove(E1, G1, WHITE);
        board.printBoard();
        EXPECT_EQ(board.makeMove(E8, G8, BLACK), "Invalid move 5957");
}


TEST(CastleTests, InvalidKSCastleMovedH8Rook){
	Board board = Board();

	
	board.makeMove(E2, E4, WHITE);
	board.makeMove(E7, E5, BLACK);
	board.makeMove(F1, C4, WHITE);
	board.makeMove(G8, F6, BLACK);
	board.makeMove(G1, F3, WHITE);
	board.makeMove(F8, C5, BLACK);
	board.makeMove(D2, D4, WHITE);
	board.makeMove(H8, G8, BLACK);
	board.makeMove(G1, F3, WHITE);
	board.makeMove(G8, H8, BLACK);
	board.makeMove(E1, G1, WHITE);
	board.printBoard();	
	EXPECT_EQ(board.makeMove(E8, G8, BLACK), "Invalid move 5957");	
}

TEST(CastleTests, BlackQueensideCastle){
	Board board = Board();

	board.makeMove(E2, E4, WHITE);
	board.makeMove(E7, E5, BLACK);
	board.makeMove(G1, F3, WHITE);
	board.makeMove(B8, C6, BLACK);
	board.makeMove(D2, D3, WHITE);
	board.makeMove(D7, D5, BLACK);
	board.makeMove(C1, E3, WHITE);
	board.makeMove(C8, G4, BLACK);
	board.makeMove(F1, E2, WHITE);
	board.makeMove(D8, D7, BLACK);
	board.makeMove(B2, B3, WHITE);
	board.printBoard();
	EXPECT_EQ(board.makeMove(E8, C8, BLACK), "Piece type = 0 from = 59 to = 61");
	board.printBoard();
}


TEST(CastleTests, BishopPreventsBlackQueensideCastle){
	Board board = Board();

	
	board.makeMove(E2, E4, WHITE);
	board.makeMove(E7, E5, BLACK);
	board.makeMove(G1, F3, WHITE);
	board.makeMove(B8, C6, BLACK);
	board.makeMove(D2, D3, WHITE);
	board.makeMove(D7, D5, BLACK);
	board.makeMove(C1, G5, WHITE);
	board.makeMove(C8, G4, BLACK);
	board.makeMove(F1, E2, WHITE);
	board.makeMove(D8, D7, BLACK);
	board.makeMove(B2, B3, WHITE);
	board.printBoard();
	EXPECT_EQ(board.makeMove(E8, C8, BLACK), "Invalid move 5961");
	board.printBoard();
}

TEST(CastleTests, InvalidQSMovedBlackKing){
        Board board = Board();

        board.makeMove(E2, E4, WHITE);
	board.makeMove(E7, E5, BLACK);
	board.makeMove(G1, F3, WHITE);
	board.makeMove(B8, C6, BLACK);
	board.makeMove(D2, D3, WHITE);
	board.makeMove(D7, D5, BLACK);
	board.makeMove(C1, E3, WHITE);
	board.makeMove(C8, G4, BLACK);
	board.makeMove(F1, E2, WHITE);
	board.makeMove(D8, D7, BLACK);
        board.makeMove(B2, B3, WHITE);
        board.makeMove(E8, D8, BLACK);
	board.makeMove(C2, C4, WHITE);
	board.makeMove(D8, E8, BLACK);
	board.makeMove(C4, C5, WHITE);
	board.printBoard();
	EXPECT_EQ(board.makeMove(E8, C8, BLACK), "Invalid move 5961");
        board.printBoard();
}

TEST(CastleTests, InvalidQSMovedA8Rook){
        Board board = Board();

        board.makeMove(E2, E4, WHITE);
	board.makeMove(E7, E5, BLACK);
	board.makeMove(G1, F3, WHITE);
	board.makeMove(B8, C6, BLACK);
	board.makeMove(D2, D3, WHITE);
	board.makeMove(D7, D5, BLACK);
	board.makeMove(C1, E3, WHITE);
	board.makeMove(C8, G4, BLACK);
	board.makeMove(F1, E2, WHITE);
	board.makeMove(D8, D7, BLACK);
        board.makeMove(B2, B3, WHITE);
        board.makeMove(E8, D8, BLACK);
	board.makeMove(C2, C4, WHITE);
	board.makeMove(D8, E8, BLACK);
	board.makeMove(C4, C5, WHITE);
	board.printBoard();
	EXPECT_EQ(board.makeMove(E8, C8, BLACK), "Invalid move 5961");
}

TEST(CastleTests, WhitePromotionMoves){
	PieceArgs args {.w_pawns_bb = RANK_7, .b_king_bb = bitset(A2), .b_queens_bb = bitset(A1)};
	Board board = Board(args);
	
	EXPECT_EQ(board.makeMove(A7, A8, WHITE, QUEEN_PROMOTION), "Promoting 55 pawn to 1");
	
	board.makeMove(A1, B1, BLACK);

	EXPECT_EQ(board.makeMove(E7, E8, WHITE, ROOK_PROMOTION), "Promoting 51 pawn to 2");
	
	board.makeMove(B1, A1, BLACK);
	
	EXPECT_EQ(board.makeMove(F7, F8, WHITE, BISHOP_PROMOTION), "Promoting 50 pawn to 3");
	
	board.makeMove(A1, B1, BLACK);
	
	EXPECT_EQ(board.makeMove(G7, G8, WHITE, KNIGHT_PROMOTION), "Promoting 49 pawn to 4");
		
	board.makeMove(B1, A1, BLACK);

	EXPECT_EQ(board.makeMove(H7, H8, WHITE), "Promoting 48 pawn to 1");
	
	board.printBoard();		
	
	EXPECT_EQ(board.getMoveCount(), 32);
}

TEST(CastleTests, WhitePromotionCaptures){
	PieceArgs args {.w_pawns_bb = bitset(A7) | bitset(E7) | bitset(F7) | bitset(B7) | bitset(H7), .b_king_bb = bitset(A2), .b_queens_bb = bitset(D8) | bitset(E8) | bitset(G8) | bitset(C8)};
        Board board = Board(args);

	board.printBoard();

        EXPECT_EQ(board.makeMove(A7, A8, WHITE, QUEEN_PROMOTION), "Promoting 55 pawn to 1");

        board.makeMove(A1, B1, BLACK);

        EXPECT_EQ(board.makeMove(E7, D8, WHITE, ROOK_PROMOTION), "Promoting 51 pawn to 2");

        board.makeMove(B1, A1, BLACK);

        
	EXPECT_EQ(board.makeMove(F7, E8, WHITE, BISHOP_PROMOTION), "Promoting 50 pawn to 3");

        board.makeMove(A1, B1, BLACK);

        EXPECT_EQ(board.makeMove(B7, C8, WHITE, KNIGHT_PROMOTION), "Promoting 54 pawn to 4");

        board.makeMove(B1, A1, BLACK);

        EXPECT_EQ(board.makeMove(H7, G8, WHITE), "Promoting 48 pawn to 1");
        board.printBoard();

        //EXPECT_EQ(board.getMoveCount(), 32);
}


TEST(CastleTests, BlackPromotionMoves){
	PieceArgs args {.w_king_bb = bitset(A8), .b_pawns_bb = RANK_2, .b_queens_bb = bitset(B8)};
	Board board = Board(args);

	board.printBoard();	
	board.makeMove(A8, A7, WHITE);
	
	EXPECT_EQ(board.makeMove(A2, A1, BLACK, QUEEN_PROMOTION), "Promoting 15 pawn to 1");
	
	board.makeMove(A7, A8, WHITE);
	
	EXPECT_EQ(board.makeMove(E2, E1, BLACK, ROOK_PROMOTION), "Promoting 11 pawn to 2");
	
	
	board.makeMove(A8, A7, WHITE);
	EXPECT_EQ(board.makeMove(F2, F1, BLACK, BISHOP_PROMOTION), "Promoting 10 pawn to 3");
	
	
	board.makeMove(A7, A8, WHITE);
	
	EXPECT_EQ(board.makeMove(G2, G1, BLACK, KNIGHT_PROMOTION), "Promoting 9 pawn to 4");
		
	
	board.makeMove(A8, A7, WHITE);

	EXPECT_EQ(board.makeMove(H2, H1, BLACK), "Promoting 8 pawn to 1");
	
	board.printBoard();		
	
	//EXPECT_EQ(board.getMoveCount(), 32);
}


TEST(CastleTests, BlackPromotionCaptures){
	PieceArgs args {.w_king_bb = bitset(A8), .w_queens_bb = bitset(C1) | bitset(D1) | bitset(F1) | bitset(G1),.b_pawns_bb = bitset(A2) | bitset(B2) | bitset(C2) | bitset(E2) | bitset(F2), .b_queens_bb = bitset(B8)};
	
	Board board = Board(args);

	board.printBoard();	
	
	board.makeMove(A8, A7, WHITE);
	
	EXPECT_EQ(board.makeMove(A2, A1, BLACK, QUEEN_PROMOTION), "Promoting 15 pawn to 1");
	
	board.makeMove(A7, A8, WHITE);
	
	EXPECT_EQ(board.makeMove(B2, C1, BLACK, ROOK_PROMOTION), "Promoting 14 pawn to 2");
	
	
	board.makeMove(A8, A7, WHITE);
	
	EXPECT_EQ(board.makeMove(C2, D1, BLACK, BISHOP_PROMOTION), "Promoting 13 pawn to 3");
	
	
	board.makeMove(A7, A8, WHITE);
	
	EXPECT_EQ(board.makeMove(E2, F1, BLACK, KNIGHT_PROMOTION), "Promoting 11 pawn to 4");
		
	
	board.makeMove(A8, A7, WHITE);

	EXPECT_EQ(board.makeMove(F2, G1, BLACK), "Promoting 10 pawn to 1");
	
	board.printBoard();		
	
}
