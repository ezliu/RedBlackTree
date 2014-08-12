#include "RedBlackTree.h"
#include "gtest/gtest.h"
#include <iostream>
#include <cstdlib>
#include <set>
#include <map>
#include <algorithm>

using namespace std;

class RedBlackTreeTest: public ::testing::Test {
	protected:
		RedBlackTree<int> myTree;
		void redChildTest();
		void blackRootTest();
		void parentChildMatchTest();
		void blackHeightTest();
};

void RedBlackTreeTest::parentChildMatchTest() {
	int num_insert = 1000;
	int modulo = 300;
	cout << "Inserting " << num_insert << " random integers [0, " 
	     << modulo-1 << "] into tree and verifying parent-child pointers.\n";
	for (int i = 0; i < 1000; i++) {
		myTree.insert(rand()%100);
		EXPECT_TRUE(myTree.parentChildMatch());
	}
}

void RedBlackTreeTest::blackHeightTest() {
	cout << "Inserting 1000 random integers [0, 199] into tree and verifying that black height is same for left and right branches for all nodes.\n";
	for (int i = 0; i < 1000; i++) {
		EXPECT_TRUE(myTree.verifyBlackHeight());
		int next = rand()%100;
		myTree.insert(next);
	}
}

void RedBlackTreeTest::blackRootTest() {
	cout << "Inserting 1000 random integers [0, 99] into tree and verifying that root is black.\n";
	for (int i = 0; i < 1000; i++) {
		EXPECT_TRUE(myTree.blackRoot());
		int next = rand()%100;
		myTree.insert(next);
	}
}

void RedBlackTreeTest::redChildTest() {
	cout << "Inserting 1000 random integers [0, 4999] into tree and verifying that there red nodes only have black parents.\n";
	queue<int> order;
	for (int i = 0; i < 1000; i++) {
		ASSERT_TRUE(myTree.verifyRedChild()) << "There are " << i << " elements in this tree.\n";
		int next = rand()%5000;
		myTree.insert(next);
		order.push(next);
	}
}

TEST_F(RedBlackTreeTest, SanityCheck) {
	cout << "Checking initial tree is size 0 and empty.\n";
	EXPECT_EQ(0, myTree.size());
	EXPECT_EQ(true, myTree.empty());
	cout << "Inserting element and checking non-zero tree size and is not empty.\n";
	myTree.insert(1);
	EXPECT_EQ(1, myTree.size());
	EXPECT_FALSE(myTree.empty());
}

TEST_F(RedBlackTreeTest, InsertionSizeTest) {
	cout << "Inserting 1000 random integers [0, 99] into tree and checking size.\n";
	for (int i = 1; i <= 1000; i++) {
		int next = rand()%100;
		myTree.insert(next);
		EXPECT_FALSE(myTree.empty());
		EXPECT_EQ(i, myTree.size());
	}
}

TEST_F(RedBlackTreeTest, BasicContainsTest) {
	cout << "Inserting integers 1 to 100 into tree\n";
	for (int i = 1; i <= 100; i++) {
		myTree.insert(i);
	}
	cout << "Checking that the tree has integers 1 to 100\n";
	for (int i = 1; i <= 100; i++) {
		EXPECT_TRUE(myTree.contains(i)) << i;
	}
}

TEST_F(RedBlackTreeTest, AdvancedContainsTest) {
	cout << "Inserting 1000 random integers [0, 299] into tree.\n";
	set<int> inserted_nums;
	for (int i = 0; i < 1000; i++) {
		int next = rand()%300;
		myTree.insert(next);
		inserted_nums.insert(next);
	}
	cout << "Checking if tree correctly contains numbers [0, 299].\n";
	for (int i = 0; i <= 299; i++) {
		bool should_be_in = false;
		if (inserted_nums.count(i) == 1) should_be_in = true;
		EXPECT_EQ(should_be_in, myTree.contains(i));
	}
}

TEST_F(RedBlackTreeTest, ParentChildMatchTest) {
	parentChildMatchTest();
}

TEST_F(RedBlackTreeTest, BlackRootTest) {
	blackRootTest();
}

TEST_F(RedBlackTreeTest, BlackHeightTest) {
	blackHeightTest();
}

TEST_F(RedBlackTreeTest, RedChildTest) {
	redChildTest();
}

TEST_F(RedBlackTreeTest, CountTest) {
	cout << "Inserting 1000 random integers [0, 299] into tree and checking counter operator.\n";
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
	cout << "Inserting 10000 random integers [0, 999] into tree and verifying all tree properties.\n";
	for (int i = 0; i < 10000; i++) {
		EXPECT_TRUE(myTree.verifyProperties());
		myTree.insert(rand()%1000);
	}
}


TEST_F(RedBlackTreeTest, DeleteExceptionTest) {
	for (int i = 0; i < 1000; i++) {
		EXPECT_THROW(myTree.remove(i), invalid_argument);
	}
}

TEST_F(RedBlackTreeTest, BasicDeleteTest) {
	cout << "Inserting 1 into the tree 1000 times." << endl;
	for (int i = 0; i < 1000; i++) {
		myTree.insert(1);
	}

	cout << "Deleting 1 from the tree 1000 times and verifying tree properties." << endl;
	for (int i = 0; i < 1000; i++) {
		myTree.remove(1);
		EXPECT_TRUE(myTree.verifyProperties());
	}
}

TEST_F(RedBlackTreeTest, DeleteTest) {
	cout << "Inserting integers 0 to 999 into tree." << endl;
	for (int i = 0; i < 1000; i++) {
		myTree.insert(i);
	}

	int counter = 0;

	cout << "Deleting integers 0 to 999 in backwards order and verifying all properties." << endl;
	for (int i = 999; i >= 0; i--) {
		myTree.remove(i);
		if (myTree.verifyProperties()) counter++;
	}

	EXPECT_EQ(1000, counter);
}

TEST_F(RedBlackTreeTest, ComprehensiveDeleteTest) {
	map<int, int> in_tree;
	vector<int> order;
	cout << "Inserting 10000 random integers [0, 30000) into tree." << endl;
	for (int i = 0; i < 10000; i++) {
		int next = rand()%30000;
		order.push_back(next);
		in_tree[next]++;
		myTree.insert(next);
	}

	random_shuffle(order.begin(), order.end());

	cout << "Deleting inserted integers in random order, and verifying that delete works properly" << endl;
	for (int i = 0; i < 10000; i++) {
		int next = order.back();
		order.pop_back();
		if (in_tree[next] != 0) {
			--in_tree[next];
			myTree.remove(next);
			EXPECT_TRUE(myTree.verifyProperties()) << "Count: " << i << endl << "Number: " << next << endl << "Amount-1: " << in_tree[next] << endl;
		} else {
			EXPECT_THROW(myTree.remove(next), invalid_argument);
		}
	}
}

TEST_F(RedBlackTreeTest, ComprehensiveTest) {
	for (int j = 0; j < 10; j++) {
		cout << "Inserting 10000 random integers [0, 30000) into tree." << endl;
		map<int, int> in_tree;
		vector<int> order;
		for (int i = 0; i < 10000; i++) {
			int next = rand()%30000;
			order.push_back(next);
			in_tree[next]++;
			myTree.insert(next);
		}

		EXPECT_FALSE(myTree.empty());
		EXPECT_EQ(10000, myTree.size());
		EXPECT_TRUE(myTree.verifyProperties());

		random_shuffle(order.begin(), order.end());
		
		cout << "Deleting inserted integers in random order" << endl;

		for (int i = 0; i < 10000; i++) {
			int next = order.back();
			order.pop_back();
			if (in_tree[next] != 0) {
				--in_tree[next];
				myTree.remove(next);
			} else {
				EXPECT_THROW(myTree.remove(next), invalid_argument);
			}
		}
		EXPECT_TRUE(myTree.empty());
		EXPECT_EQ(0, myTree.size());
		EXPECT_TRUE(myTree.verifyProperties());
	}
}

class ConstructorTests: public ::testing::Test {
	protected:
		RedBlackTree<int> myTree;
		virtual void SetUp();
		virtual void TearDown();
		map<int, int> elems_in_tree;
		static int const k_max = 3000;
};

void ConstructorTests::SetUp () {
	for (int i = 0; i < 1000; i++) {
		int next = rand()%k_max;
		++elems_in_tree[next];
		myTree.insert(next);
	}
}

void ConstructorTests::TearDown() {
	for (auto map_it=elems_in_tree.begin(); map_it != elems_in_tree.end(); ++map_it) {
		while (map_it->second > 0) {
			--map_it->second;
			myTree.remove(map_it->first);
		}
	}
}

TEST_F(ConstructorTests, BasicCopyConstructorTest) {
	cout << "Making copy using copy constructor and testing copy.\n";
	RedBlackTree<int> copy(myTree);
	EXPECT_FALSE(copy.size() == 0);
	for (int i = 0; i < k_max; i++) EXPECT_EQ(copy.count(i), elems_in_tree[i]);
}

TEST_F(ConstructorTests, CopyConstructorIndependenceTest) {
	cout << "Testing that changing copy does not change original.\n";
	RedBlackTree<int> copy(myTree);
	map<int, int> copy_elems_in_tree = elems_in_tree;
	for (int i = 0; i < k_max; i++) {
		while (copy_elems_in_tree[i] > 0) {
			copy.remove(i);
			--copy_elems_in_tree[i];
		}
	}
	EXPECT_TRUE(copy.empty());

	for (int i = 0; i < k_max; i++) EXPECT_EQ(myTree.count(i), elems_in_tree[i]);
}

TEST_F(ConstructorTests, BasicAssignmentOperatorTest) {
	cout << "Making a copy using assignment operator and checking that the copy worked." << endl;
	RedBlackTree<int> copy;
	copy = myTree;
	for (int i = 0; i < k_max; i++) EXPECT_EQ(copy.count(i), elems_in_tree[i]);
}

TEST_F(ConstructorTests, AssignmentOperatorEdgeCases) {
	cout << "Inserting 1000 random integers into a RBTree" << endl;
	RedBlackTree<int> copy;
	for (int i = 0; i < 1000; i++) copy.insert(rand()%k_max);

	cout << "Making a copy of that RB Tree" << endl;
	RedBlackTree<int> copy_two = copy;

	cout << "Copying the original tree and checking that everything was copied properly." << endl;
	copy = myTree;
	for (int i = 0; i < k_max; i++) EXPECT_EQ(copy.count(i), elems_in_tree[i]);
	
	cout << "Inserting 1000 new random integers into the copied tree" << endl;
	for (int i = 0; i < 1000; i++) copy.insert(rand()%k_max);

	cout << "Testing that we can string together multiply assignment operations" << endl;
	copy_two = copy = myTree;

	for (int i = 0; i < k_max; i++) EXPECT_EQ(copy.count(i), elems_in_tree[i]);
	for (int i = 0; i < k_max; i++) EXPECT_EQ(copy_two.count(i), elems_in_tree[i]);
	for (int i = 0; i < k_max; i++) EXPECT_EQ(myTree.count(i), elems_in_tree[i]);

	EXPECT_EQ(copy.size(), myTree.size());
	EXPECT_EQ(copy.size(), copy_two.size());

	cout << "Deleting all elements of copy and checking that it doesn't affect others" << endl;
	copy.clear();

	ASSERT_EQ(0, copy.size());
	ASSERT_EQ(1000, copy_two.size());
	ASSERT_EQ(1000, myTree.size());
	for (int i = 0; i < k_max; i++) EXPECT_EQ(copy_two.count(i), elems_in_tree[i]);
	for (int i = 0; i < k_max; i++) EXPECT_EQ(myTree.count(i), elems_in_tree[i]);
}


int main(int argc, char **argv) {
	srand(time(NULL));
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

