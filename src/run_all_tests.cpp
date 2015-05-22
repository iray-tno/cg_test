#include "gtest/gtest.h"
#include "cg_test.h"

TEST(MultiTwoTest, MultiTwo){
  EXPECT_EQ(4, multi_two(2));
  EXPECT_EQ(6, multi_two(3));
  EXPECT_EQ(22222, multi_two(11111));
}

int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}