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

TEST(MakeMoveTests, KingSideCastle){
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
