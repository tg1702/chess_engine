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
