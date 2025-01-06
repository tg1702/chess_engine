#include <gtest/gtest.h>
//#include "move_tests.h"
//#include "board_tests.h"
#include "perft_tests.h"

TEST(Test2, t){
	EXPECT_EQ(20, 20);
}
int main(int argc, char **argv){
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();

}
