#include "RedBlackTree.h"
#include "gtest/gtest.h"

using namespace std;

class RedBlackTreeTest: public ::testing::Test {
	protected:
		virtual void setUp();
		virtual void tearDown();
};

void RedBlackTreeTest::setUp() {

}

void RedBlackTreeTest::tearDown() {
	
}

TEST_F(RedBlackTreeTest, BasicTest) {
	EXPECT_EQ(0, 0);
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

