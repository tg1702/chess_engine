
#include <gtest/gtest.h>
#include "../src/perft.h"
#include "../src/board.h"

TEST(CPWPerfts, StartPos){
	Board board = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	EXPECT_EQ(p_perft(1, board), 20);
	EXPECT_EQ(p_perft(2, board), 400);
	EXPECT_EQ(p_perft(3, board), 8902);
	EXPECT_EQ(p_perft(4, board), 197281);
	EXPECT_EQ(p_perft(5, board), 4865609);
	EXPECT_EQ(p_perft(6, board), 119060324);
}

TEST(CPWPerfts, Kiwipete){
	Board board = Board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
	EXPECT_EQ(p_perft(1, board), 48);
	EXPECT_EQ(p_perft(2, board), 2039);
	EXPECT_EQ(p_perft(3, board), 97862);
	EXPECT_EQ(p_perft(4, board), 4085603);
	EXPECT_EQ(p_perft(5, board), 193690690);
}

TEST(CPWPerfts, Position3){
	Board board = Board("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
	EXPECT_EQ(p_perft(1, board), 14);
	EXPECT_EQ(p_perft(2, board), 191);
	EXPECT_EQ(p_perft(3, board), 2812);
	EXPECT_EQ(p_perft(4, board), 43238);
	EXPECT_EQ(p_perft(5, board), 674624);
	EXPECT_EQ(p_perft(6, board), 11030083);
}

TEST(CPWPerfts, Position4){
	Board board = Board("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");

	EXPECT_EQ(p_perft(1, board), 6);
	EXPECT_EQ(p_perft(2, board), 264);
	EXPECT_EQ(p_perft(3, board), 9467);
	EXPECT_EQ(p_perft(4, board), 422333);
	EXPECT_EQ(p_perft(5, board), 15833292);

}

TEST(CPWPerfts, Position5){
	Board board = Board("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
	EXPECT_EQ(p_perft(1, board), 44);
	EXPECT_EQ(p_perft(2, board), 1486);
	EXPECT_EQ(p_perft(3, board), 62379);
	EXPECT_EQ(p_perft(4, board), 2103487);
	EXPECT_EQ(p_perft(5, board), 89941194);
}

TEST(CPWPerfts, Position6){
	Board board = Board("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
	EXPECT_EQ(p_perft(1, board), 46);
	EXPECT_EQ(p_perft(2, board), 2079);
	EXPECT_EQ(p_perft(3, board), 89890);
	EXPECT_EQ(p_perft(4, board), 3894594);
	EXPECT_EQ(p_perft(5, board), 164075551);
}

TEST(TalkChess, IllegalEP1){
	Board board = Board("3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 1");

	EXPECT_EQ(p_perft(6, board), 1134888);
}

TEST(TalkChess, IllegalEP2){
	Board board = Board("8/8/4k3/8/2p5/8/B2P2K1/8 w - - 0 1");
	EXPECT_EQ(p_perft(6, board), 1015133);

}

TEST(TalkChess, EPCaptureCheck){
	Board board = Board("8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 1");
	EXPECT_EQ(p_perft(6,board), 1440467);
}

TEST(TalkChess, ShortCastleCheck){
	Board board = Board("5k2/8/8/8/8/8/8/4K2R w K - 0 1");
	EXPECT_EQ(p_perft(6, board), 661072);
}

TEST(TalkChess, LongCastleCheck){
	Board board = Board("3k4/8/8/8/8/8/8/R3K3 w Q - 0 1");
	EXPECT_EQ(p_perft(6, board), 803711);
}

TEST(TalkChess, CastleRights){
	Board board = Board("r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1");
	EXPECT_EQ(p_perft(4, board), 1274206);
}

TEST(TalkChess, CastlingPrevented){
	Board board = Board("r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq - 0 1");
	EXPECT_EQ(p_perft(4, board), 1720476);
}

TEST(TalkChess, PromoteOutOfCheck){
	Board board = Board("2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 1");
	EXPECT_EQ(p_perft(6, board), 3821001);
}

TEST(TalkChess, DiscoveredCheck){
	Board board = Board("8/8/1P2K3/8/2n5/1q6/8/5k2 b - - 0 1");
	EXPECT_EQ(p_perft(5, board), 1004658);
}

TEST(TalkChess, PromoteToCheck){
	Board board = Board("4k3/1P6/8/8/8/8/K7/8 w - - 0 1");
	EXPECT_EQ(p_perft(6, board), 217342); 
}

TEST(TalkChess, UnderPromoteToCheck){
	Board board = Board("8/P1k5/K7/8/8/8/8/8 w - - 0 1");
	EXPECT_EQ(p_perft(6, board), 92683);
}

TEST(TalkChess, SelfStalemate){
	Board board = Board("K1k5/8/P7/8/8/8/8/8 w - - 0 1");
	EXPECT_EQ(p_perft(6, board), 2217);
}

TEST(TalkChess, StaleAndCheckMate){
	Board board = Board("8/k1P5/8/1K6/8/8/8/8 w - - 0 1");
	EXPECT_EQ(p_perft(7, board), 567584);
}

TEST(TalkChess, StaleAndCheckMate2){
	Board board = Board("8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1");
	EXPECT_EQ(p_perft(4, board), 23527);
}
