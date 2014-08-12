#include "RedBlackTree.h"
#include "gtest/gtest.h"
#include <iostream>
#include <cstdlib>
#include <set>
#include <map>

using namespace std;

class RedBlackTreeTest: public ::testing::Test {
	protected:
		RedBlackTree<int> myTree;
		virtual void SetUp();
		virtual void TearDown();
};

void RedBlackTreeTest::SetUp() {

}

void RedBlackTreeTest::TearDown() {
	
}

namespace testing {

	AssertionResult AssertionSuccess();

	AssertionResult AssertionFailure();


}

::testing::AssertionResult IsZero(int n) {
	if (n == 0) return ::testing::AssertionSuccess() << n << " is zero.";
	else return::testing::AssertionFailure() << n << " is not zero.";
}

TEST_F(RedBlackTreeTest, SanityCheck) {
	std::cout << "Checking initial tree is size 0 and empty.\n";
	EXPECT_EQ(0, myTree.size());
	EXPECT_EQ(true, myTree.empty());
	std::cout << "Inserting element and checking non-zero tree size and is not empty.\n";
	myTree.insert(1);
	EXPECT_EQ(1, myTree.size());
	EXPECT_FALSE(myTree.empty());
}

TEST_F(RedBlackTreeTest, InsertionSizeTest) {
	std::cout << "Inserting 1000 random integers [0, 99] into tree and checking size.\n";
	for (int i = 1; i <= 1000; i++) {
		int next = rand()%100;
		myTree.insert(next);
		EXPECT_FALSE(myTree.empty());
		EXPECT_EQ(i, myTree.size());
	}
}

TEST_F(RedBlackTreeTest, BasicContainsTest) {
	std::cout << "Inserting integers 1 to 100 into tree\n";
	for (int i = 1; i <= 100; i++) {
		myTree.insert(i);
	}
	std::cout << "Checking that the tree has integers 1 to 100\n";
	for (int i = 1; i <= 100; i++) {
		EXPECT_TRUE(myTree.contains(i)) << i;
	}
}

TEST_F(RedBlackTreeTest, AdvancedContainsTest) {
	std::cout << "Inserting 1000 random integers [0, 299] into tree.\n";
	set<int> inserted_nums;
	for (int i = 0; i < 1000; i++) {
		int next = rand()%300;
		myTree.insert(next);
		inserted_nums.insert(next);
	}
	std::cout << "Checking if tree correctly contains numbers [0, 299].\n";
	for (int i = 0; i <= 299; i++) {
		bool should_be_in = false;
		if (inserted_nums.count(i) == 1) should_be_in = true;
		EXPECT_EQ(should_be_in, myTree.contains(i));
	}
}

TEST_F(RedBlackTreeTest, ParentChildMatchTest) {
	std::cout << "Inserting 1000 random integers [0, 99] into tree and verifying parent-child pointers.\n";
	for (int i = 0; i < 1000; i++) {
		myTree.insert(rand()%100);
		EXPECT_TRUE(myTree.parentChildMatch());
	}
}

TEST_F(RedBlackTreeTest, BlackRootTest) {
	std::cout << "Inserting 1000 random integers [0, 99] into tree and verifying that root is black.\n";
	for (int i = 0; i < 1000; i++) {
		EXPECT_TRUE(myTree.blackRoot());
		int next = rand()%100;
		myTree.insert(next);
	}
}

TEST_F(RedBlackTreeTest, BlackHeightTest) {
	std::cout << "Inserting 1000 random integers [0, 199] into tree and verifying that black height is same for left and right branches for all nodes.\n";
	for (int i = 0; i < 1000; i++) {
		EXPECT_TRUE(myTree.verifyBlackHeight());
		int next = rand()%100;
		myTree.insert(next);
	}
}

TEST_F(RedBlackTreeTest, RedChildTest) {
	std::cout << "Inserting 1000 random integers [0, 4999] into tree and verifying that there red nodes only have black parents.\n";
	queue<int> order;
	for (int i = 0; i < 1000; i++) {
		ASSERT_TRUE(myTree.verifyRedChild()) << "There are " << i << " elements in this tree.\n";
		int next = rand()%5000;
		myTree.insert(next);
		order.push(next);
	}
}

TEST_F(RedBlackTreeTest, CountTest) {
	std::cout << "Inserting 1000 random integers [0, 299] into tree and checking counter operator.\n";
	map<int, int> freq;
	for (int i = 0; i < 1000; i++) {
		int next = rand()%300;
		freq[next]++;
		myTree.insert(next);
	}

	for (int i = 0; i < 300; i++) {
		EXPECT_EQ(freq[i], myTree.count(i));
	}
}

TEST_F(RedBlackTreeTest, FullInsertTest) {
	std::cout << "Inserting 10000 random integers [0, 999] into tree and verifying all tree properties.\n";
	for (int i = 0; i < 10000; i++) {
		EXPECT_TRUE(myTree.verifyBlackHeight());
		EXPECT_TRUE(myTree.verifyRedChild());
		EXPECT_TRUE(myTree.blackRoot());
		EXPECT_TRUE(myTree.parentChildMatch());
		myTree.insert(rand()%1000);
	}
}


TEST_F(RedBlackTreeTest, DeleteExceptionTest) {
	for (int i = 0; i < 1000; i++) {
		EXPECT_THROW(myTree.remove(i), std::invalid_argument);
	}
}

TEST_F(RedBlackTreeTest, TrivialDeleteTest) {
	for (int i = 0; i < 1000; i++) {
		myTree.insert(1);
	}

	for (int i = 0; i < 1000; i++) {
		myTree.remove(1);
		EXPECT_TRUE(myTree.verifyBlackHeight());
		EXPECT_TRUE(myTree.verifyRedChild());
		EXPECT_TRUE(myTree.parentChildMatch());
	}
}

TEST_F(RedBlackTreeTest, SlightlyLessTrivialDeleteTest) {
	for (int i = 0; i < 1000; i++) {
		myTree.insert(i);
	}

	int counter = 0;

	for (int i = 0; i < 1000; i++) {
		//myTree.print();
		myTree.remove(i);
		if (myTree.verifyBlackHeight() && myTree.verifyRedChild() && myTree.parentChildMatch()) {
			counter++;
			std::cout << "Correct!" << std::endl;
		}
	}

	EXPECT_EQ(10, counter);
}

int main(int argc, char **argv) {
	srand(time(NULL));
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

