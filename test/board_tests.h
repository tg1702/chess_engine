#include <iostream>
#include <gtest/gtest.h>
#include <array>
#include "../src/types.h"
#include "../src/board.h"

TEST(MakeMoveTests, InitialBoardMoves){
	Board board = Board();
	
	std::array <unsigned int, 256> moveTests = { 80, 82, 405, 407, 528, 536, 593, 601, 658, 666, 723, 731, 788, 796, 853, 861, 918, 926, 983, 991, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 
	
	//EXPECT_EQ(board.getMove(), moveTests);
	EXPECT_EQ(board.getMoveCount(), 20);

}

TEST(Captures, NormalCaptures){
	PieceArgs args {.w_knights_bb = bitset(F3), .b_pawns_bb = bitset(E5)};

	Board board = Board(args);

	EXPECT_EQ(board.makeMove(F3, E5), "f3e5"); 
}

TEST(Captures, PawnCaptures){
	PieceArgs args {.w_pawns_bb = bitset(E4), .b_rooks_bb = bitset(D5)};
	Board board = Board(args);

	board.printBoard();
	EXPECT_EQ(board.makeMove(E4, D5), "e4d5");
	board.printBoard();

}
TEST(CastleTests, WhiteKingSideCastle){
	Board board = Board();
		
	board.makeMove(E2, E4);
	board.makeMove(E7, E5);
	board.makeMove(F1, C4);
	board.makeMove(G8, F6);
	board.makeMove(G1, F3);
	board.makeMove(B8, C6);	
	EXPECT_EQ(board.makeMove(E1, G1), "e1g1");

}

TEST(CastleTests, InvalidKSCastleMovedWhiteKing){
	Board board = Board();

	
	board.makeMove(E2, E4);
	board.makeMove(E7, E5);
	board.makeMove(F1, C4);
	board.makeMove(G8, F6);
	board.makeMove(G1, F3);
	board.makeMove(B8, C6);
	board.makeMove(E1, E2);
	board.makeMove(B8, C6);
	board.makeMove(E2, E1);
	board.makeMove(A7, A5);

	board.printBoard();	
	EXPECT_EQ(board.makeMove(E1, G1), "Invalid move e1g1");	
}

TEST(CastleTests, BishopAttacksG1PreventsKSCastle){
        Board board = Board();


        board.makeMove(E2, E4);
        board.makeMove(E7, E5);
        board.makeMove(F1, C4);
        board.makeMove(G8, F6);
        board.makeMove(F2, F4);
        board.makeMove(F8, C5);
        board.makeMove(G1, F3);
        board.makeMove(A7, A5);

        board.printBoard();
        EXPECT_EQ(board.makeMove(E1, G1), "Invalid move e1g1");
}


TEST(CastleTests, InvalidKSCastleMovedH1Rook){
	Board board = Board();

	
	board.makeMove(E2, E4);
	board.makeMove(E7, E5);
	board.makeMove(F1, C4);
	board.makeMove(G8, F6);
	board.makeMove(G1, F3);
	board.makeMove(B8, C6);
	board.makeMove(H1, G1);
	board.makeMove(B8, C6);
	board.makeMove(G1, H1);
	board.makeMove(A7, A5);

	board.printBoard();	
	EXPECT_EQ(board.makeMove(E1, G1), "Invalid move e1g1");	
}

TEST(CastleTests, WhiteQueensideCastle){
	Board board = Board();

	board.makeMove(E2, E4);
	board.makeMove(E7, E5);
	board.makeMove(B1, C3);
	board.makeMove(B8, C6);
	board.makeMove(D2, D3);
	board.makeMove(F8, C5);
	board.makeMove(C1, E3);
	board.makeMove(G8, F6);
	board.makeMove(D1, D2);
	board.makeMove(A7, A5);

	board.printBoard();
	EXPECT_EQ(board.makeMove(E1, C1), "e1c1");
	board.printBoard();
}


TEST(CastleTests, BishopPreventsWhiteQueensideCastle){
	Board board = Board();

	cout << board.makeMove(E2, E4) << endl;
	cout << board.makeMove(E7, E5) << endl;
	cout << board.makeMove(B1, C3) << endl;
	cout << board.makeMove(B8, C6) << endl;
	cout << board.makeMove(D2, D3) << endl;
	cout << board.makeMove(F8, E7) << endl;
	cout << board.makeMove(D1, F3) << endl;
	cout << board.makeMove(D7, D5) << endl;
	cout << board.makeMove(C1, H6) << endl;
	cout << board.makeMove(E7, G5) << endl;

	board.printBoard();
	EXPECT_EQ(board.makeMove(E1, C1), "Invalid move e1c1");
	board.printBoard();
}

TEST(CastleTests, InvalidQSMovedKing){
        Board board = Board();

        board.makeMove(E2, E4);
        board.makeMove(E7, E5);
        board.makeMove(B1, C3);
        board.makeMove(B8, C6);
        board.makeMove(D2, D3);
        board.makeMove(F1, C5);
        board.makeMove(C1, E3);
        board.makeMove(G8, F6);
        board.makeMove(D1, D2);
        board.makeMove(A7, A5);
	board.makeMove(E1, D1);
	board.makeMove(A5, A4);
	board.makeMove(D1, E1);
	board.makeMove(F6, G4);
        board.printBoard();
        EXPECT_EQ(board.makeMove(E1, C1), "Invalid move e1c1");
        board.printBoard();
}

TEST(CastleTests, InvalidQSMovedA1Rook){
        Board board = Board();

        board.makeMove(E2, E4);
        board.makeMove(E7, E5);
        board.makeMove(B1, C3);
        board.makeMove(B8, C6);
        board.makeMove(D2, D3);
        board.makeMove(F1, C5);
        board.makeMove(C1, E3);
        board.makeMove(G8, F6);
        board.makeMove(D1, D2);
        board.makeMove(A7, A5);
        board.makeMove(A1, B1);
        board.makeMove(A5, A4);
        board.makeMove(B1, A1);
        board.makeMove(F6, G4);
        board.printBoard();
        EXPECT_EQ(board.makeMove(E1, C1), "Invalid move e1c1");
        board.printBoard();
}

/* BLACK */
TEST(CastleTests, BlackKingSideCastle){
	Board board = Board();
		
	board.makeMove(E2, E4);
	board.makeMove(E7, E5);
	board.makeMove(F1, C4);
	board.makeMove(G8, F6);
	board.makeMove(G1, F3);
	board.makeMove(F8, C5);	
	board.makeMove(E1, G1);	
	EXPECT_EQ(board.makeMove(E8, G8), "e8g8");

}

TEST(CastleTests, InvalidKSCastleMovedBlackKing){
	Board board = Board();

	
	board.makeMove(E2, E4);
	board.makeMove(E7, E5);
	board.makeMove(F1, C4);
	board.makeMove(G8, F6);
	board.makeMove(G1, F3);
	board.makeMove(F8, C5);
	board.makeMove(D2, D4);
	board.makeMove(E8, F8);
	board.makeMove(A2, A4);
	board.makeMove(F8, E8);
	board.makeMove(H2, H4);
	board.printBoard();	
	EXPECT_EQ( board.makeMove(E8, G8), "Invalid move e8g8");	
}

TEST(CastleTests, BishopAttacksG8PreventsKSCastle){
        Board board = Board();


        board.makeMove(E2, E4);
        board.makeMove(E7, E5);
        board.makeMove(F1, C4);
        board.makeMove(F7, F5);
        board.makeMove(D2, D3);
        board.makeMove(F8, C5);
        board.makeMove(G1, F3);
        board.makeMove(G8, F6);
	board.makeMove(E1, G1);
        board.printBoard();
        EXPECT_EQ(board.makeMove(E8, G8), "Invalid move e8g8");
}


TEST(CastleTests, InvalidKSCastleMovedH8Rook){
	Board board = Board();

	
	board.makeMove(E2, E4);
	board.makeMove(E7, E5);
	board.makeMove(F1, C4);
	board.makeMove(G8, F6);
	board.makeMove(G1, F3);
	board.makeMove(F8, C5);
	board.makeMove(D2, D4);
	board.makeMove(H8, G8);
	board.makeMove(G1, F3);
	board.makeMove(G8, H8);
	board.makeMove(E1, G1);
	board.printBoard();	
	EXPECT_EQ(board.makeMove(E8, G8), "Invalid move e8g8");	
}

TEST(CastleTests, BlackQueensideCastle){
	Board board = Board();

	board.makeMove(E2, E4);
	board.makeMove(E7, E5);
	board.makeMove(G1, F3);
	board.makeMove(B8, C6);
	board.makeMove(D2, D3);
	board.makeMove(D7, D5);
	board.makeMove(C1, E3);
	board.makeMove(C8, G4);
	board.makeMove(F1, E2);
	board.makeMove(D8, D7);
	board.makeMove(B2, B3);
	board.printBoard();
	EXPECT_EQ(board.makeMove(E8, C8), "e8c8");
	board.printBoard();
}


TEST(CastleTests, BishopPreventsBlackQueensideCastle){
	Board board = Board();

	
	board.makeMove(E2, E4);
	board.makeMove(E7, E5);
	board.makeMove(G1, F3);
	board.makeMove(B8, C6);
	board.makeMove(D2, D3);
	board.makeMove(D7, D5);
	board.makeMove(C1, G5);
	board.makeMove(C8, G4);
	board.makeMove(F1, E2);
	board.makeMove(D8, D7);
	board.makeMove(B2, B3);
	board.printBoard();
	EXPECT_EQ(board.makeMove(E8, C8), "Invalid move e8c8");
	board.printBoard();
}

TEST(CastleTests, InvalidQSMovedBlackKing){
        Board board = Board();

        board.makeMove(E2, E4);
	board.makeMove(E7, E5);
	board.makeMove(G1, F3);
	board.makeMove(B8, C6);
	board.makeMove(D2, D3);
	board.makeMove(D7, D5);
	board.makeMove(C1, E3);
	board.makeMove(C8, G4);
	board.makeMove(F1, E2);
	board.makeMove(D8, D7);
        board.makeMove(B2, B3);
        board.makeMove(E8, D8);
	board.makeMove(C2, C4);
	board.makeMove(D8, E8);
	board.makeMove(C4, C5);
	board.printBoard();
	EXPECT_EQ(board.makeMove(E8, C8), "Invalid move e8c8");
        board.printBoard();
}

TEST(CastleTests, InvalidQSMovedA8Rook){
        Board board = Board();

        board.makeMove(E2, E4);
	board.makeMove(E7, E5);
	board.makeMove(G1, F3);
	board.makeMove(B8, C6);
	board.makeMove(D2, D3);
	board.makeMove(D7, D5);
	board.makeMove(C1, E3);
	board.makeMove(C8, G4);
	board.makeMove(F1, E2);
	board.makeMove(D8, D7);
        board.makeMove(B2, B3);
        board.makeMove(E8, D8);
	board.makeMove(C2, C4);
	board.makeMove(D8, E8);
	board.makeMove(C4, C5);
	board.printBoard();
	EXPECT_EQ(board.makeMove(E8, C8), "Invalid move e8c8");
}

TEST(PromotionTests, WhitePromotionMoves){
	PieceArgs args {.w_pawns_bb = RANK_7, .b_king_bb = bitset(A2), .b_queens_bb = bitset(A1)};
	Board board = Board(args);

	board.printBoard();

	EXPECT_EQ(board.makeMove(A7, A8, QUEEN_PROMOTION), "a7a8");
	
	board.makeMove(A1, B1);

	EXPECT_EQ(board.makeMove(E7, E8, ROOK_PROMOTION), "e7e8");
	
	board.printBoard();
	board.makeMove(B1, A1);
	
	EXPECT_EQ(board.makeMove(F7, F8, BISHOP_PROMOTION), "f7f8");
	
	board.makeMove(A1, B1);
	
	EXPECT_EQ(board.makeMove(G7, G8, KNIGHT_PROMOTION), "g7g8");

		
	board.makeMove(B1, A1);

	EXPECT_EQ(board.makeMove(H7, H8), "h7h8");
	
	board.printBoard();		
	
	EXPECT_EQ(board.getMoveCount(), 18);
}

TEST(PromotionTests, WhitePromotionCaptures){
	PieceArgs args {.w_pawns_bb = bitset(A7) | bitset(E7) | bitset(F7) | bitset(B7) | bitset(H7), .b_king_bb = bitset(A1), .b_queens_bb = bitset(D8) | bitset(E8) | bitset(G8) | bitset(C8)};
        Board board = Board(args);

	board.printBoard();

        EXPECT_EQ(board.makeMove(A7, A8, QUEEN_PROMOTION), "a7a8");

        board.makeMove(A1, B1);

        EXPECT_EQ(board.makeMove(E7, D8, ROOK_PROMOTION), "e7d8");

        board.makeMove(B1, A1);

        
	EXPECT_EQ(board.makeMove(F7, E8, BISHOP_PROMOTION), "f7e8");

        board.makeMove(A1, B1);

        EXPECT_EQ(board.makeMove(B7, C8, KNIGHT_PROMOTION), "b7c8");

        board.makeMove(B1, A1);

        EXPECT_EQ(board.makeMove(H7, G8), "h7g8");
        board.printBoard();

}


TEST(PromotionTests, BlackPromotionMoves){
	PieceArgs args {.w_king_bb = bitset(A8), .b_pawns_bb = RANK_2, .b_queens_bb = bitset(B8)};
	Board board = Board(args);

	board.printBoard();	
	board.makeMove(A8, A7);
	
	EXPECT_EQ(board.makeMove(A2, A1, QUEEN_PROMOTION), "a2a1");
	
	board.makeMove(A7, A8);
	
	EXPECT_EQ(board.makeMove(E2, E1, ROOK_PROMOTION), "e2e1");
	
	board.makeMove(A8, A7);
	
	board.printBoard();

	EXPECT_EQ(board.makeMove(F2, F1, BISHOP_PROMOTION), "f2f1");
	
	
	board.makeMove(A7, A8);
	
	EXPECT_EQ(board.makeMove(G2, G1, KNIGHT_PROMOTION), "g2g1");
		
	
	board.makeMove(A8, A7);

	EXPECT_EQ(board.makeMove(H2, H1), "h2h1");
	
	board.printBoard();		
	
}


TEST(PromotionTests, BlackPromotionCaptures){
	PieceArgs args {.w_king_bb = bitset(A8), .w_queens_bb = bitset(C1) | bitset(D1) | bitset(F1) | bitset(G1),.b_pawns_bb = bitset(A2) | bitset(B2) | bitset(C2) | bitset(E2) | bitset(F2), .b_queens_bb = bitset(B8)};
	
	Board board = Board(args);

	board.printBoard();	
	
	board.makeMove(A8, A7);
	
	EXPECT_EQ(board.makeMove(A2, A1, QUEEN_PROMOTION), "a2a1");
	
	board.makeMove(A7, A8);
	
	EXPECT_EQ(board.makeMove(B2, C1, ROOK_PROMOTION), "b2c1");
	
	
	board.makeMove(A8, A7);
	
	EXPECT_EQ(board.makeMove(C2, D1, BISHOP_PROMOTION), "c2d1");
	
	
	board.makeMove(A7, A8);
	
	EXPECT_EQ(board.makeMove(E2, F1, KNIGHT_PROMOTION), "e2f1");
		
	
	board.makeMove(A8, A7);

	EXPECT_EQ(board.makeMove(F2, G1), "f2g1");
	
	board.printBoard();		
	
}

TEST(EnPassantTests, WhiteCanEnPassant){
	Board board = Board();
	board.makeMove(D2, D4);
	board.makeMove(B7, B5);
	board.makeMove(D4, D5);
	board.makeMove(E7, E5);

	board.printBoard();	
	EXPECT_EQ(board.makeMove(D5, E6), "d5e6");
	board.printBoard();
}


TEST(EnPassantTests, WhiteMissesEnPassant){
	Board board = Board();
	board.makeMove(D2, D4);
	board.makeMove(B7, B5);
	board.makeMove(D4, D5);
	board.makeMove(E7, E5);
	board.makeMove(G1, F3);
	board.makeMove(F8, E7);
	board.printBoard();	
	EXPECT_EQ(board.makeMove(D5, E6), "Invalid move d5e6");
	board.printBoard();

	
	cout << board.makeMove(D5, D6) << endl;
	board.makeMove(A7, A5);
	board.printBoard();
        EXPECT_EQ(board.makeMove(D6, C7), "d6c7");
        board.printBoard();
	
}


TEST(EnPassantTests, BlackCanEnPassant){
        Board board = Board();
        board.makeMove(D2, D4);
        board.printBoard();
	board.makeMove(B7, B5);
        board.printBoard();
	board.makeMove(D4, D5);
        board.printBoard();
	board.makeMove(B5, B4);
	board.printBoard();
	board.makeMove(C2, C4);
        
	board.printBoard();
        EXPECT_EQ(board.makeMove(B4, C3), "b4c3");
        board.printBoard();
}


TEST(EnPassantTests, BlackMissesEnPassant){
        Board board = Board();
        board.makeMove(D2, D4);
        board.printBoard();
	board.makeMove(B7, B5);
        board.printBoard();
	board.makeMove(D4, D5);
        board.printBoard();
	board.makeMove(B5, B4);
	board.printBoard();
	board.makeMove(C2, C4);
	board.printBoard();
	board.makeMove(A7, A5);
       	board.printBoard();
	board.makeMove(D1, D4);	
	board.printBoard();
        EXPECT_EQ(board.makeMove(B4, C3), "Invalid move b4c3");
        board.printBoard();
}


TEST(UndoTests, UndoNormalMove){
        Board board = Board();
        board.makeMove(D2, D4);
        board.printBoard();
	EXPECT_EQ(board.makeMove(B7, B6), "b7b6");
        board.printBoard();
	EXPECT_EQ(board.makeMove(D4, D5), "d4d5");
        board.printBoard();
	EXPECT_EQ(board.makeMove(B6, B5), "b6b5");
	board.printBoard();
	EXPECT_EQ(board.makeMove(C2, C4), "c2c4");
	board.printBoard();
	EXPECT_EQ(board.unmakeMove(), "Undoing normal move c2c4");
	board.printBoard();
}

TEST(UndoMoves, NormalCaptures){
        PieceArgs args {.w_knights_bb = bitset(F3), .b_pawns_bb = bitset(E5)};

        Board board = Board(args);
	board.printBoard();
        board.makeMove(F3, E5);
	board.printBoard();
	EXPECT_EQ(board.unmakeMove(), "Undoing capture move f3e5");
	board.printBoard();
}

TEST(UndoMoves, PawnCaptures){
	PieceArgs args {.w_pawns_bb = bitset(E4), .b_rooks_bb = bitset(D5)};
	Board board = Board(args);

	board.printBoard();
	board.makeMove(E4, D5);
	board.printBoard();
	EXPECT_EQ(board.unmakeMove(), "Undoing capture move e4d5");
	board.printBoard();

}

TEST(UndoTests, WhiteCanEnPassant){
        Board board = Board();
        board.makeMove(D2, D4);
        board.makeMove(B7, B5);
        board.makeMove(D4, D5);
        board.makeMove(E7, E5);

        board.printBoard();
        board.makeMove(D5, E6);
        board.printBoard();
	EXPECT_EQ(board.unmakeMove(), "Undoing en passant capture d5e6");
	board.printBoard();
}


TEST(UndoTests, BlackCanEnPassant){
        Board board = Board();
        board.makeMove(D2, D4);
        board.printBoard();
	board.makeMove(B7, B5);
        board.printBoard();
	board.makeMove(D4, D5);
        board.printBoard();
	board.makeMove(B5, B4);
	board.printBoard();
	board.makeMove(C2, C4);
        
	board.printBoard();
        board.makeMove(B4, C3);
        board.printBoard();
	EXPECT_EQ(board.unmakeMove(), "Undoing en passant capture b4c3");
	board.printBoard();
}

TEST(UndoTests, WhiteKingSideCastle){
        Board board = Board();

        board.makeMove(E2, E4);
        board.makeMove(E7, E5);
        board.makeMove(F1, C4);
        board.makeMove(G8, F6);
        board.makeMove(G1, F3);
        board.makeMove(B8, C6);
        board.makeMove(E1, G1);
	board.printBoard();
	EXPECT_EQ(board.unmakeMove(), "Undoing white kingside castle e1g1");
	board.printBoard();
}

TEST(UndoTests, WhiteQueensideCastle){
        Board board = Board();

        board.makeMove(E2, E4);
        board.makeMove(E7, E5);
        board.makeMove(B1, C3);
        board.makeMove(B8, C6);
        board.makeMove(D2, D3);
        board.makeMove(F8, C5);
        board.makeMove(C1, E3);
        board.makeMove(G8, F6);
        board.makeMove(D1, D2);
        board.makeMove(A7, A5);

        board.printBoard();
        board.makeMove(E1, C1);
        board.printBoard();
	EXPECT_EQ(board.unmakeMove(), "Undoing white queenside castle e1c1");
	board.printBoard();
}

TEST(UndoTests, BlackKingSideCastle){
        Board board = Board();

        board.makeMove(E2, E4);
        board.makeMove(E7, E5);
        board.makeMove(F1, C4);
        board.makeMove(G8, F6);
        board.makeMove(G1, F3);
        board.makeMove(F8, C5);
        board.makeMove(E1, G1);
        board.printBoard();
	board.makeMove(E8, G8);
	board.printBoard();	
	EXPECT_EQ(board.unmakeMove(), "Undoing black kingside castle e8g8");
	board.printBoard();
}

TEST(UndoTests, BlackQueensideCastle){
        Board board = Board();

        board.makeMove(E2, E4);
        board.makeMove(E7, E5);
        board.makeMove(G1, F3);
        board.makeMove(B8, C6);
        board.makeMove(D2, D3);
        board.makeMove(D7, D5);
        board.makeMove(C1, E3);
        board.makeMove(C8, G4);
        board.makeMove(F1, E2);
        board.makeMove(D8, D7);
        board.makeMove(B2, B3);
        board.printBoard();
        board.makeMove(E8, C8);
	board.printBoard();
	EXPECT_EQ(board.unmakeMove(), "Undoing black queenside castle e8c8");
        board.printBoard();
}


TEST(UndoTests, WhitePromotionCaptures){                                                                                   PieceArgs args {.w_pawns_bb = bitset(A7) | bitset(E7) | bitset(F7) | bitset(B7) | bitset(H7), .b_king_bb = bitset(A1), .b_queens_bb = bitset(D8) | bitset(E8) | bitset(G8) | bitset(C8)};
        Board board = Board(args);

        board.printBoard();

        EXPECT_EQ(board.makeMove(A7, A8, QUEEN_PROMOTION), "a7a8");

        board.makeMove(A1, B1);

        EXPECT_EQ(board.makeMove(E7, D8, ROOK_PROMOTION), "e7d8");

        board.makeMove(B1, A1);


        EXPECT_EQ(board.makeMove(F7, E8, BISHOP_PROMOTION), "f7e8");

        board.makeMove(A1, B1);

        EXPECT_EQ(board.makeMove(B7, C8, KNIGHT_PROMOTION), "b7c8");

        board.makeMove(B1, A1);

        EXPECT_EQ(board.makeMove(H7, G8), "h7g8");
        
	EXPECT_EQ(board.unmakeMove(), "Undoing queen promotion capture h7g8");

		
	EXPECT_EQ(board.unmakeMove(), "Undoing normal move b1a1");


	EXPECT_EQ(board.unmakeMove(), "Undoing knight promotion capture b7c8");

		
	EXPECT_EQ(board.unmakeMove(), "Undoing normal move a1b1");

	EXPECT_EQ(board.unmakeMove(), "Undoing bishop promotion capture f7e8");
	
	EXPECT_EQ(board.unmakeMove(), "Undoing normal move b1a1");
	EXPECT_EQ(board.unmakeMove(), "Undoing rook promotion capture e7d8");
	
	EXPECT_EQ(board.unmakeMove(), "Undoing normal move a1b1");
	
	EXPECT_EQ(board.unmakeMove(), "Undoing queen promotion a7a8");
	board.printBoard();

}


