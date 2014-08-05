#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H
#define NULL 0

#include <sstream>
#include <queue>
#include <iostream>

/** Red Black Tree as implemented by Evan Liu
 *
 * Satisfies the following (MIT OpenCourseware as resource):
 *
 * 1) Each node is red or black.
 * 2) Root and NULL leaves are black.
 * 3) Each red node must have two black child nodes.
 * 4) Any path from a node to its descendant leaves has the same number of
 * black nodes.
 */


template <typename ElemType>
class RedBlackTree {
public:
    RedBlackTree();
    /** Destructor */
    ~RedBlackTree();
    
    /** Inserts an element */
    void insert(const ElemType& value);

    /** Returns the number of elements in the tree */
    int size() const;

    /** Returns if the tree is empty or not */
    bool empty() const;

    /** Returns a debug stringstream */
//    std::stringstream& debugString() const;

    /** Prints out the tree */
    void print() const;

    /** Recursive wrapper for verifying red nodes have only black children */
    bool verifyRedChild() const;

    /** Recursive wrapper for verifying all black heights are equal */
    bool verifyBlackHeight() const;

    /** Checks if an element is in the tree */
    bool contains(const ElemType& value) const;

    /** Returns the number of times an element is in the tree. */
    int count(const ElemType& value) const;

    /** Checks if the root is black */
    bool blackRoot() const;

    /** Checks if parents and children pointers match */
    bool parentChildMatch() const;

    /** Deletes a node corresponding to the value if it exists in the tree */
    void remove(const ElemType &value);
private:
    //TODO: Move this back into private
    typedef struct Node {
	    Node* parent;
	    ElemType value;
	    bool red; // False => Black
	    Node* lChild;
	    Node* rChild;
	    std::size_t count; // For any duplicates 
    } Node;

    Node* root;
    int numElems;

    /** Recursively inserts a new value to the tree */
    void recursiveInsert(const ElemType& value, Node* currNode);

    /** Makes a new node with value equal to value */
    Node* makeNode(const ElemType& value, Node* parent, const bool red = true) const;

    /** Recursively deletes the tree */
    void recursiveDelete(Node*& currNode);

    /** Performs a single rotation */
    void rotate(Node* child, const bool left);

    /** Returns the grandparent node */
    Node* grandparent(const Node* child);

    /** Returns the Uncle node */
    Node* uncle(const Node* child);

    /** Returns sibling node */
    Node* sibling(const Node* child);

    /** Fixes any errors in coloring from insertion/deletion */
    void restoreTree(Node* child);

    /** For debugging purposes: verifies that red nodes have only black children */
    bool verifyRedChild(Node* currNode) const;

    /** For debugging purposes: returns the blackheight of a given node recursively */
    //TODO: Use memoization
    int blackHeight(Node* currNode, const bool left) const;

    /** Recursive function to check all blackheights */
    bool verifyBlackHeight(Node* currNode) const;

    /** Recursively checks that parent and children pointers match */
    bool parentChildMatch(Node* currNode) const;

    /** Finds the node in the tree with the given value if it exists. Returns NULL if not. */
    Node* findNode(Node* currNode, const ElemType &value) const;
};

/**
 * Implementation details */

/** Constructor */
template <typename ElemType>
RedBlackTree<ElemType>::RedBlackTree():
	root(NULL),
	numElems(0)
{}

/** Destructor */
template <typename ElemType>
RedBlackTree<ElemType>::~RedBlackTree() {
	recursiveDelete(root);
}

/** Recursive wrapper for insert */
template <typename ElemType>
void RedBlackTree<ElemType>::insert(const ElemType &value) {
	numElems++;
	recursiveInsert(value, root);
}

/** Returns number of keys in tree */
template <typename ElemType>
int RedBlackTree<ElemType>::size() const {
	return numElems;
}

/** Returns if tree is empty */
template <typename ElemType>
bool RedBlackTree<ElemType>::empty() const {
	return size() == 0;
}

//TODO: Implement a DEBUG STRING 


/** Prints out the tree by enqueueing all of the elements in order by 
 * tree level */
template <typename ElemType>
void RedBlackTree<ElemType>::print() const {
	std::queue<Node*> myQueue;
	myQueue.push(root); // Start with root
	std::queue<Node*> nextQueue;
	bool allNull = true;
	while (true) {
		Node* nextNode = myQueue.front();
		myQueue.pop();
		if (nextNode == NULL) { // Enqueue root node 
			nextQueue.push(NULL); // Put in two NULL children
			nextQueue.push(NULL);
			std::cout << "NULL (b,0) ";
		} else {
			Node* lChild = nextNode->lChild; //Enqueue both children nodes 
			Node* rChild = nextNode->rChild;
			if (lChild == NULL) nextQueue.push(NULL);
			else nextQueue.push(nextNode->lChild);
			if (rChild == NULL) nextQueue.push(NULL);
			else nextQueue.push(nextNode->rChild);
			std::cout << nextNode->value; // Print currNode
			std::cout << " (" << ((nextNode->red) ? "r," : "b,");
			std::cout << nextNode->count << ") ";
			allNull = false;
		}
		if (myQueue.empty()) {
			std::cout << std::endl;
			if (allNull) return; // Continue until all leaf nodes
			allNull = true;
			std::swap(myQueue, nextQueue); // Move onto next level of tree
		}
	}
}

/** Recursively inserts an element into tree and then restores the tree properties */
template <typename ElemType>
void RedBlackTree<ElemType>::recursiveInsert(const ElemType& value, Node* currNode) {
	if (root == NULL) { // Insert root node
		root = makeNode(value, NULL, false);
	} else if (value < currNode->value) { // Traverse left
		if (currNode->lChild == NULL) { // If insertable, place node.
			currNode->lChild = makeNode(value, currNode);
			restoreTree(currNode->lChild);
		} else recursiveInsert(value, currNode->lChild); 	
	} else if (value == currNode->value) {
	       	currNode->count++; // Duplicates insert
	} else { // Traverse right
		if (currNode->rChild == NULL) { // If insertable, place node.
			currNode->rChild = makeNode(value, currNode);
			restoreTree(currNode->rChild);
		} else recursiveInsert(value, currNode->rChild);
	}
}

/** Makes an initial node with the given params */
template <typename ElemType>
typename RedBlackTree<ElemType>::Node*
RedBlackTree<ElemType>::makeNode(const ElemType& value, Node* parent, const bool red) const {
	Node* newNode = new Node;
	newNode->parent = parent;
	newNode->red = red;
	newNode->lChild = NULL;
	newNode->rChild = NULL;
	newNode->value = value;
	newNode->count = 1;
	return newNode;
}

/** Frees memory of current node and all of its children recursively*/
template <typename ElemType>
void RedBlackTree<ElemType>::recursiveDelete(Node*& currNode) {
	if (currNode == NULL) return;
	recursiveDelete(currNode->lChild);
	recursiveDelete(currNode->rChild);
	delete currNode;
}

/** Does a tree rotation at given node*/
template <typename ElemType>
void RedBlackTree<ElemType>::rotate(Node* child, const bool left) {
	Node* origParent = child->parent; // Store original pointers
	Node* origGrandparent = origParent->parent;
	child->parent = origGrandparent;
	origParent->parent = child;
	Node* origGrandchild;
	if (left) { // Left rotate 
		origGrandchild = child->lChild;
		child->lChild = origParent;
		origParent->rChild = origGrandchild;
	} else { // Right rotate
		origGrandchild = child->rChild;
		child->rChild = origParent;
		origParent->lChild = origGrandchild;
	}
	if (origGrandchild != NULL) origGrandchild->parent = origParent; // Reconnect grandchild
	if (origGrandparent == NULL) {
		root = child;
		return;
	}
	if (origGrandparent->lChild == origParent) origGrandparent->lChild = child; // Connect nodes back to grandparent
	else origGrandparent->rChild = child;
}

/** Returns grandparent node */
template <typename ElemType>
typename RedBlackTree<ElemType>::Node*
RedBlackTree<ElemType>::grandparent(const Node* child) {
	if (child == NULL || child->parent == NULL) return NULL;
	return child->parent->parent;
}

/** Returns uncle node */
template <typename ElemType>
typename RedBlackTree<ElemType>::Node*
RedBlackTree<ElemType>::uncle(const Node* child) {
	if (grandparent(child) == NULL) return NULL;
	if (grandparent(child)->rChild == child->parent) return grandparent(child)->lChild;
	return grandparent(child)->rChild;
}

/** Returns sibling node */
template <typename ElemType>
typename RedBlackTree<ElemType>::Node*
RedBlackTree<ElemType>::sibling(const Node* child) {
	if (child == NULL || child->parent == NULL) return NULL;
	if (child->parent->lChild == child) return child->parent->rChild;
	return child->parent->lChild;
}

/** Gets called to restore red black properties */
template <typename ElemType>
void RedBlackTree<ElemType>::restoreTree(Node* child) {
	// NOTE: This should not get called on a NULL node, so child should never be NULL.
	if (root->red == true) { // Root is wrong color.
		root->red = false;
		return;
	} 
	if (child->red && !child->parent->red) return; // Case I. Regular insertion. 
	if (child->red && child->parent->red) {
		if (uncle(child) != NULL && uncle(child)->red) { // Case II. Parent && Uncle are red.
			grandparent(child)->red = true; // Solution: Swap colors from grand gen to par gen
			uncle(child)->red = false;
			child->parent->red = false;
			restoreTree(grandparent(child)); // Fix any issues that this might have caused.
		} else {
			/** Case III. Child is opposite child that parent is and parent/uncle diff colors */
			/** Solution: Rotate so that parent and child are both lChild or rChild.
			 * Finish by doing Case IV */
			// NOTE: Grandparent should never be NULL if there's a red red discrepancy!
			if (grandparent(child) == NULL) std::cout << "NO!" << std::endl;
			if (grandparent(child)->lChild == child->parent && child == child->parent->rChild) {
				Node* origParent = child->parent;
				rotate(child, true); // Left rotate
				restoreTree(origParent); // Proceed to Case IV with new child.
			} else if (grandparent(child)->rChild == child->parent && child == child->parent->lChild) {
				Node* origParent = child->parent;
				rotate(child, false); // Right rotate
				restoreTree(origParent); // Case IV
			} else { // Case IV: Parent and Child are on same side.
				/** Solution: Rotate parent and grandparent and then swap colors */
				Node* origParent = child->parent;
				Node* origGrandparent = grandparent(child);
				origParent->red = origGrandparent->red;
				origGrandparent->red = !origGrandparent->red;
				if (origParent == origGrandparent->lChild) rotate(origParent, false);
				else rotate(origParent, true);
			}
		}
	}
}

/** Recursive wrapper for verifying red nodes have only black children */
template <typename ElemType>
bool RedBlackTree<ElemType>::verifyRedChild() const {
	return verifyRedChild(root);
}

/** Verifies the property that red nodes have black children recursively starting from currNode */
template <typename ElemType>
bool RedBlackTree<ElemType>::verifyRedChild(Node* currNode) const {
	if (currNode == NULL) return true;
	if (currNode->lChild == NULL && currNode->rChild == NULL) return true;
	if (((currNode->lChild != NULL && currNode->lChild->red) ||
	     (currNode->rChild != NULL && currNode->rChild->red)) && currNode->red) return false;
	if (!verifyRedChild(currNode->lChild) || !verifyRedChild(currNode->rChild)) return false;
	return true;
}

/** Returns black height for a given node */
template <typename ElemType>
int RedBlackTree<ElemType>::blackHeight(Node* currNode, const bool left) const {
	if (left) {
		if (currNode->lChild == NULL) return 1;
		if (currNode->lChild->red) return blackHeight(currNode->lChild, left);
		return blackHeight(currNode->lChild, left)+1;
	} else {
		if (currNode->rChild == NULL) return 1;
		if (currNode->rChild->red) return blackHeight(currNode->rChild, left);
		return blackHeight(currNode->rChild, left)+1;
	}
}

/** Recursively verifies that node has equal left and right black heights */
template <typename ElemType>
bool RedBlackTree<ElemType>::verifyBlackHeight(Node* currNode) const {
	if (currNode == NULL) return true;
	if (blackHeight(currNode, true) != blackHeight(currNode, false)) return false; 
	if (!verifyBlackHeight(currNode->lChild) || !verifyBlackHeight(currNode->rChild)) return false;
	return true;
}

/** Recursive wrapper for verifying black height */
template <typename ElemType>
bool RedBlackTree<ElemType>::verifyBlackHeight() const {
	return verifyBlackHeight(root);
}

/** Recursive wrapper for determining if an element is contained in the tree */
template <typename ElemType>
bool RedBlackTree<ElemType>::contains(const ElemType& value) const {
	return count(value) != 0;
}

/** Returns whether or not the root is black. Returns true if NULL root */
template <typename ElemType>
bool RedBlackTree<ElemType>::blackRoot() const {
	if (root == NULL) return true;
	return !root->red;	
}

/** Recursive wrapper for verifying that parent's children are children's parents */
template <typename ElemType>
bool RedBlackTree<ElemType>::parentChildMatch() const {
	return parentChildMatch(root);
}

/** Recursively checks for match between parent and child pointers */
template <typename ElemType>
bool RedBlackTree<ElemType>::parentChildMatch(Node* currNode) const {
	if (currNode == NULL) return true;
	if (currNode->lChild != NULL && currNode->lChild->parent != currNode) return false;
	if (currNode->rChild != NULL && currNode->rChild->parent != currNode) return false;
	if (!parentChildMatch(currNode->lChild) || !parentChildMatch(currNode->rChild)) return false;
	return true;
}

/** Finds a given node if it exists. Returns NULL otherwise. */
template <typename ElemType>
typename RedBlackTree<ElemType>::Node*
RedBlackTree<ElemType>::findNode(Node* currNode, const ElemType &value) const {
	if (currNode == NULL) return NULL;
	if (currNode->value == value) return currNode;
	if (value < currNode->value) return findNode(currNode->lChild, value);
	else return findNode(currNode->rChild, value);
}

/** Returns the number of times a key is in the tree. */
template <typename ElemType>
int RedBlackTree<ElemType>::count(const ElemType &value) const {
	if (findNode(root, value) == NULL) return 0;
	return findNode(root, value)->count;
}

/** Deletes an element from the tree if it exists. Otherwise, it throws an error */
template <typename ElemType>
void RedBlackTree<ElemType>::remove(const ElemType &value) {
	Node* toDelete = findNode(root, value);
	if (toDelete == NULL) throw std::invalid_argument("That value is not in the tree.");
}
#endif // REDBLACKTREE_H
