#include <iostream>
#include <gtest/gtest.h>

#ifndef PIECES_H
#define PIECES_H
#endif

#ifndef TYPES_H
#define TYPES_H
#endif

#include "../src/board.h"

TEST(MakeMoveTests, PawnMoves){
	Board board = Board();
	EXPECT_EQ(board.getMove(), 0);

}
