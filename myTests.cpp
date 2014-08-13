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
		void FullInsertTest();
		void BasicDeleteTest();
		void DeleteTest();
		void ComprehensiveDeleteTest();
		void ComprehensiveTest();
		void HighDensityDeleteTest();

};

void RedBlackTreeTest::parentChildMatchTest() {
	int num_insert = 1000;
	int modulo = 300;
	cout << "Inserting " << num_insert << " random integers [0, " 
	     << modulo-1 << "] into tree and verifying parent-child pointers.\n";
	for (int i = 0; i < num_insert; i++) {
		myTree.insert(rand()%modulo);
		EXPECT_TRUE(myTree.parentChildMatch());
	}
}

void RedBlackTreeTest::blackHeightTest() {
	int num_insert = 1000;
	int modulo = 300;
	cout << "Inserting " << num_insert << " random integers [0, " 
	     << modulo-1 << "] into tree and verifying black height is the same for left and right "
	     << "branches for all nodes.\n";
	for (int i = 0; i < num_insert; i++) {
		EXPECT_TRUE(myTree.verifyBlackHeight());
		int next = rand()%modulo;
		myTree.insert(next);
	}
}

void RedBlackTreeTest::blackRootTest() {
	int num_insert = 1000;
	int modulo = 300;
	cout << "Inserting " << num_insert << " random integers [0, " 
	     << modulo-1 << "] into tree and verifying that the root is always black.\n";

	for (int i = 0; i < num_insert; i++) {
		EXPECT_TRUE(myTree.blackRoot());
		int next = rand()%modulo;
		myTree.insert(next);
	}
}

void RedBlackTreeTest::redChildTest() {
	int num_insert = 1000;
	int modulo = 5000;
	cout << "Inserting " << num_insert << " random integers [0, " 
	     << modulo-1 << "] into tree and verifying that red nodes only have black parents.\n";

	queue<int> order;
	for (int i = 0; i < num_insert; i++) {
		ASSERT_TRUE(myTree.verifyRedChild()) << "There are " << i << " elements in this tree.\n";
		int next = rand()%modulo;
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
	int num_insert = 1000;
	int modulo = 300;
	cout << "Inserting " << num_insert << " random integers [0, " 
	     << modulo-1 << "] into tree and verifying that and verifying size.\n";

	for (int i = 1; i <= num_insert; i++) {
		int next = rand()%modulo;
		myTree.insert(next);
		EXPECT_FALSE(myTree.empty());
		EXPECT_EQ(i, myTree.size());
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

void RedBlackTreeTest::FullInsertTest() {
	int num_insert = 10000;
	int modulo = 50000;
	cout << "Inserting " << num_insert << " random integers [0, " 
	     << modulo-1 << "] into tree and verifying all tree properties.\n";

	for (int i = 0; i < num_insert; i++) {
		EXPECT_TRUE(myTree.verifyProperties());
		myTree.insert(rand()%modulo);
	}
}

TEST_F(RedBlackTreeTest, FullInsertTest) {
	FullInsertTest();
}

TEST_F(RedBlackTreeTest, BasicContainsTest) {
	int num_insert = 100;

	cout << "Inserting integers 1 to " << num_insert << " into tree\n";
	for (int i = 1; i <= num_insert; i++) {
		myTree.insert(i);
	}
	cout << "Checking that the tree has integers 1 to " << num_insert << "\n";
	for (int i = 1; i <= num_insert; i++) {
		EXPECT_TRUE(myTree.contains(i)) << i;
	}
}

TEST_F(RedBlackTreeTest, AdvancedContainsTest) {
	int num_insert = 1000;
	int modulo = 3000;
	cout << "Inserting " << num_insert << " random integers [0, " 
	     << modulo-1 << "] into tree.\n";

	set<int> inserted_nums;
	for (int i = 0; i < num_insert; i++) {
		int next = rand()%modulo;
		myTree.insert(next);
		inserted_nums.insert(next);
	}
	cout << "Checking if tree correctly contains numbers [0, " << modulo-1 << "].\n";
	for (int i = 0; i <=  modulo; i++) {
		bool should_be_in = false;
		if (inserted_nums.count(i) == 1) should_be_in = true;
		EXPECT_EQ(should_be_in, myTree.contains(i));
	}
}

TEST_F(RedBlackTreeTest, CountTest) {
	int num_insert = 1000;
	int modulo = 2000;
	cout << "Inserting " << num_insert << " random integers [0, " 
	     << modulo-1 << "] into tree and checking count operator.\n";

	map<int, int> freq;
	for (int i = 0; i < num_insert; i++) {
		int next = rand()%modulo;
		freq[next]++;
		myTree.insert(next);
	}

	for (int i = 0; i < modulo; i++) {
		EXPECT_EQ(freq[i], myTree.count(i));
	}
}

TEST_F(RedBlackTreeTest, DeleteExceptionTest) {
	int num_check = 1000;	

	cout << "Verifying that remove throws exceptions for elements that aren't there.\n";

	for (int i = 0; i < num_check; i++) {
		EXPECT_THROW(myTree.remove(i), invalid_argument);
	}
}

void RedBlackTreeTest::BasicDeleteTest() {
	int num_to_insert = 1;
	int num_check = 1000;

	cout << "Inserting " << num_to_insert << " in the tree " << num_check << " times.\n";
	for (int i = 0; i < num_check; i++) {
		myTree.insert(num_to_insert);
	}

	cout << "Deleting " << num_to_insert << " from the tree " << num_check << " times "
	     << "and verifying tree properties.\n";

	for (int i = 0; i < num_check; i++) {
		myTree.remove(num_to_insert);
		EXPECT_TRUE(myTree.verifyProperties());
	}
}

TEST_F(RedBlackTreeTest, BasicDeleteTest) {
	BasicDeleteTest();
}

void RedBlackTreeTest::DeleteTest() {
	int num_insert = 1000;

	cout << "Inserting integers 0 to " << num_insert << " into tree.\n";
	for (int i = 0; i < num_insert; i++) {
		myTree.insert(i);
	}

	int counter = 0;

	cout << "Deleting integers 0 to " << num_insert 
	     <<	" in backwards order and verifying all properties.\n"; 

	for (int i = num_insert-1; i >= 0; i--) {
		myTree.remove(i);
		if (myTree.verifyProperties()) counter++;
	}

	EXPECT_EQ(num_insert, counter);
}

TEST_F(RedBlackTreeTest, DeleteTest) {
	DeleteTest();
}

void RedBlackTreeTest::HighDensityDeleteTest() {
	int num_insert = 10000;
	int modulo = 10000;

	cout << "Inserting " << num_insert << " random integers [0, "
	     << modulo-1 << "] into tree.\n";

	map<int, int> in_tree;
	vector<int> order;

	for (int i = 0; i < num_insert; i++) {
		int next = rand()%modulo;
		order.push_back(next);
		in_tree[next]++;
		myTree.insert(next);
	}

	random_shuffle(order.begin(), order.end());

	cout << "Deleting inserted integers in random order, and verifying that delete works properly.\n";
	for (int i = 0; i < num_insert; i++) {
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

TEST_F(RedBlackTreeTest, HighDensityDeleteTest) {
	HighDensityDeleteTest();
}

void RedBlackTreeTest::ComprehensiveDeleteTest() {
	int num_insert = 10000;
	int modulo = 30000;
	cout << "Inserting " << num_insert << " random integers [0, " 
	     << modulo-1 << "] into tree.\n";

	map<int, int> in_tree;
	vector<int> order;

	for (int i = 0; i < num_insert; i++) {
		int next = rand()%modulo;
		order.push_back(next);
		in_tree[next]++;
		myTree.insert(next);
	}

	random_shuffle(order.begin(), order.end());

	cout << "Deleting inserted integers in random order, and verifying that delete works properly.\n";
	for (int i = 0; i < num_insert; i++) {
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

TEST_F(RedBlackTreeTest, ComprehensiveDeleteTest) {
	ComprehensiveDeleteTest();
}

void RedBlackTreeTest::ComprehensiveTest() {
	int num_times = 10;
	int num_insert = 10000;
	int modulo = 30000;

	for (int j = 0; j < num_times; j++) {
		cout << "Inserting " << num_insert << " random integers [0, " 
		     << modulo-1 << "] into tree.\n";
		map<int, int> in_tree;
		vector<int> order;
		for (int i = 0; i < num_insert; i++) {
			int next = rand()%modulo;
			order.push_back(next);
			in_tree[next]++;
			myTree.insert(next);
		}

		EXPECT_FALSE(myTree.empty());
		EXPECT_EQ(num_insert, myTree.size());
		EXPECT_TRUE(myTree.verifyProperties());

		random_shuffle(order.begin(), order.end());
		
		cout << "Deleting inserted integers in random order.\n";

		for (int i = 0; i < num_insert; i++) {
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

TEST_F(RedBlackTreeTest, ComprehensiveTest) {
	ComprehensiveTest();
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
	int num_insert = 1000;

	for (int i = 0; i < num_insert; i++) {
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
	cout << "Making a copy using assignment operator and checking that the copy worked.\n";
	RedBlackTree<int> copy;
	copy = myTree;
	for (int i = 0; i < k_max; i++) EXPECT_EQ(copy.count(i), elems_in_tree[i]);
}

TEST_F(ConstructorTests, AssignmentOperatorEdgeCases) {
	int num_insert = 1000;

	cout << "Inserting " << num_insert << " random integers into a RBTree.\n";
	RedBlackTree<int> copy;
	for (int i = 0; i < num_insert; i++) copy.insert(rand()%k_max);

	cout << "Making a copy of that RB Tree" << endl;
	RedBlackTree<int> copy_two = copy;

	cout << "Copying the original tree and checking that everything was copied properly.\n";
	copy = myTree;
	for (int i = 0; i < k_max; i++) EXPECT_EQ(copy.count(i), elems_in_tree[i]);
	
	cout << "Inserting " << num_insert << " new random integers into the copied tree.\n";
	for (int i = 0; i < num_insert; i++) copy.insert(rand()%k_max);

	cout << "Testing that we can string together multiply assignment operations.\n";
	copy_two = copy = myTree;

	for (int i = 0; i < k_max; i++) EXPECT_EQ(copy.count(i), elems_in_tree[i]);
	for (int i = 0; i < k_max; i++) EXPECT_EQ(copy_two.count(i), elems_in_tree[i]);
	for (int i = 0; i < k_max; i++) EXPECT_EQ(myTree.count(i), elems_in_tree[i]);

	EXPECT_EQ(copy.size(), myTree.size());
	EXPECT_EQ(copy.size(), copy_two.size());

	cout << "Deleting all elements of copy and checking that it doesn't affect others.\n";
	copy.clear();

	ASSERT_EQ(0, copy.size());
	ASSERT_EQ(num_insert, copy_two.size());
	ASSERT_EQ(num_insert, myTree.size());
	for (int i = 0; i < k_max; i++) EXPECT_EQ(copy_two.count(i), elems_in_tree[i]);
	for (int i = 0; i < k_max; i++) EXPECT_EQ(myTree.count(i), elems_in_tree[i]);
}


int main(int argc, char **argv) {
	srand(time(NULL));
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

