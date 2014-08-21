#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H
#define NULL 0

#include <sstream>
#include <queue>
#include <iostream>

/** Copyright (c) 2014 Evan Liu
 *
 * Red Black Tree as implemented by Evan Liu.
 *
 * Satisfies the following: 
 *
 * 1) Each node is red or black.
 * 2) Root and NULL leaves are black.
 * 3) Each red node must have two black child nodes.
 * 4) Any path from a node to its descendant leaves has the same number of
 * black nodes. 
 *
 * Resources used:
 *
 * MIT OpenCourseware Lecture
 * Introduction to Algorithms (Cormen, Leierson, Rivest, Stein)
 *
 */

template <typename ElemType>
class RedBlackTree {
friend class RedBlackTreeTest;
public:
    /** Constructor */
    RedBlackTree();

    /** Copy Constructor */
    RedBlackTree(const RedBlackTree<ElemType> &other);

    /** Assignment Operator */
    RedBlackTree<ElemType>& operator= (const RedBlackTree<ElemType> &other);

    /** Destructor */
    ~RedBlackTree();
    
    /** Inserts an element */
    void insert(const ElemType& value);

    /** Returns the number of elements in the tree */
    int size() const;

    /** Returns if the tree is empty or not */
    bool empty() const;

    /** Returns a debug string */
    std::string debugString() const;

    /** Prints out the tree */
    void print() const;

    /** Checks if an element is in the tree */
    bool contains(const ElemType& value) const;

    /** Returns the number of times an element is in the tree. */
    int count(const ElemType& value) const;

    /** Deletes a node corresponding to the value if it exists in the tree */
    void remove(const ElemType &value);
    
    /** Clears the tree */
    void clear();

private:
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
    
    /** Deals with all of the delete cases recursively */
    void rbDelete(Node* currNode);

    /** Swaps counts and values */
    void swap(Node* left, Node* right);

    /** Returns in-order predecessor */
    Node* inOrderPredecessor(Node* node) const;

    /** Returns num NULL children. 0 if given node is NULL */
    int numNullChildren(Node* node) const;

    /** Restores tree properties after delete. */
    void deleteRestoreTree(Node* parent, Node* notSibling);

    /** Recursive wrapper for verifying red nodes have only black children */
    bool verifyRedChild() const;

    /** Recursive wrapper for verifying all black heights are equal */
    bool verifyBlackHeight() const;

    /** Checks if the root is black */
    bool blackRoot() const;

    /** Checks if parents and children pointers match */
    bool parentChildMatch() const;

    /** Copies a tree recursively */
    void copyTree(const Node* from, Node* &into, Node* const parent);

    /** Verfies that the sum of the counts is equal to the size */
    bool verifyCount() const;

    /** Recursively sums up the count of all descendant nodes of the currNode including
     * the currNode*/
    int countSum(Node* currNode) const;

    /** Wrapper for verifying all RB Properties */
    bool verifyProperties() const;
};

/** Implementation details */

/** Constructor */
template <typename ElemType>
RedBlackTree<ElemType>::RedBlackTree():
	root(NULL),
	numElems(0)
{}

/** Copy Constructor */
template <typename ElemType>
RedBlackTree<ElemType>::RedBlackTree(const RedBlackTree<ElemType> &other):
	numElems(other.numElems)
{
	copyTree(other.root, root, NULL);
}

/** Assignment Operator */
template <typename ElemType>
RedBlackTree<ElemType>& RedBlackTree<ElemType>::operator= (const RedBlackTree &other) {
	if (this != &other) {
		clear(); // Delete tree
		numElems = other.numElems; // Re-initialize
		copyTree(other.root, root, NULL);
	}
	return *this;
}

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

/** Clears the tree */
template <typename ElemType>
void RedBlackTree<ElemType>::clear() {
	recursiveDelete(root);
	numElems = 0;
	root = NULL;
}

/** Returns a debug string */
template <typename ElemType>
std::string RedBlackTree<ElemType>::debugString() const {
	std::stringstream converter;
	std::queue<Node*> myQueue;
	myQueue.push(root); // Start with root
	std::queue<Node*> nextQueue;
	bool allNull = true;
	while (true) { // Enqueue in order of depth
		Node* nextNode = myQueue.front();
		myQueue.pop();
		if (nextNode == NULL) { // Enqueue root node 
			nextQueue.push(NULL); // Put in two NULL children
			nextQueue.push(NULL);
			converter << "NULL (b,0) ";
		} else {
			Node* lChild = nextNode->lChild; //Enqueue both children nodes 
			Node* rChild = nextNode->rChild;
			if (lChild == NULL) nextQueue.push(NULL);
			else nextQueue.push(nextNode->lChild);
			if (rChild == NULL) nextQueue.push(NULL);
			else nextQueue.push(nextNode->rChild);
			converter << nextNode->value; // Print currNode
			converter << " (" << ((nextNode->red) ? "r," : "b,");
			converter << nextNode->count << ") ";
			allNull = false;
		}
		if (myQueue.empty()) {
			converter << std::endl;
			if (allNull) break; // Continue until all leaf nodes
			allNull = true;
			std::swap(myQueue, nextQueue); // Move onto next level of tree
		}
	}
	return converter.str();
}

/** Prints out the tree by enqueueing all of the elements in order by 
 * tree level */
template <typename ElemType>
void RedBlackTree<ElemType>::print() const {
	std::cout << debugString() << std::endl;
}

/** Recursively inserts an element into tree and then restores the tree properties
 * @value The value to insert
 * @currNode The current node
 */
template <typename ElemType>
void RedBlackTree<ElemType>::recursiveInsert(const ElemType& value, Node* currNode) {
	if (root == NULL)  // Insert root node
		root = makeNode(value, NULL, false);
	else if (value == currNode->value)
		currNode->count++; // Duplicate insert
	else {
		Node** child;
		if (value < currNode->value) child = &currNode->lChild; //Traverse l/r
		else child = &currNode->rChild;
		if (*child == NULL) { // If insertable, place node.
			*child = makeNode(value, currNode);
			restoreTree(*child);
		} else recursiveInsert(value, *child); // Otherwise, continue traversing
	}
}

/** Makes an initial node with the given params
 * @value The value of the node
 * @parent The parent of the node
 * @red The color of the node
 */
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

/** Frees memory of current node and all of its children recursively
 * @currNode The current node being deleted */
template <typename ElemType>
void RedBlackTree<ElemType>::recursiveDelete(Node*& currNode) {
	if (currNode == NULL) return; // Stop at leaves
	recursiveDelete(currNode->lChild);
	recursiveDelete(currNode->rChild);
	delete currNode;
}

/** Does a tree rotation at given node
 * @child The child node to be rotated up left or right
 * @left The direction of the rotation
 */
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
	if (origGrandparent == NULL) root = child; // If no grandparent, then at root.
	else if (origGrandparent->lChild == origParent) origGrandparent->lChild = child; // Connect nodes back to grandparent
	else origGrandparent->rChild = child;
}

/** Returns grandparent node 
 * @child The child node */
template <typename ElemType>
typename RedBlackTree<ElemType>::Node*
RedBlackTree<ElemType>::grandparent(const Node* child) {
	if (child == NULL || child->parent == NULL) return NULL;
	return child->parent->parent;
}

/** Returns uncle node
 * @child The child node */
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

/** Gets called to restore red black properties
 * @child The current node being looked at. Viewed as child */
template <typename ElemType>
void RedBlackTree<ElemType>::restoreTree(Node* child) {
	// NOTE: This should not get called on a NULL node, so child should never be NULL.
	if (root->red) { // Root is wrong color.
		root->red = false;
		return;
	} 
	if (!child->parent->red) return; // Case I. Regular insertion. 
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

/** Verifies the property that red nodes have black children recursively starting from currNode
 * @currNode The current node being verified */
template <typename ElemType>
bool RedBlackTree<ElemType>::verifyRedChild(Node* currNode) const {
	if (currNode == NULL) return true; // Stop at leaves
	if (currNode->lChild == NULL && currNode->rChild == NULL) return true; // Check cases of red-red parent/child
	if (((currNode->lChild != NULL && currNode->lChild->red) ||
	     (currNode->rChild != NULL && currNode->rChild->red)) && currNode->red) return false;
	if (!verifyRedChild(currNode->lChild) || !verifyRedChild(currNode->rChild)) return false;
	return true;
}

/** Returns black height for a given node in a given direction
 * @currNode The given node
 * @left The direction */
template <typename ElemType>
int RedBlackTree<ElemType>::blackHeight(Node* currNode, const bool left) const {
	Node* child;
	if (left) child = currNode->lChild;
	else child = currNode->rChild;
	if (child == NULL) return 1; // 1 for leaves
	if (child->red) return blackHeight(child, left); // If not black, return child's blackheight
	return blackHeight(child, left)+1; // Otherwise, add one to child's blackheight
}

/** Recursively verifies that node has equal left and right black heights
 * @currNode The current node being verified */
template <typename ElemType>
bool RedBlackTree<ElemType>::verifyBlackHeight(Node* currNode) const {
	if (currNode == NULL) return true; // Stop at leaves
	if (blackHeight(currNode, true) != blackHeight(currNode, false)) return false; // Check currNode
	if (!verifyBlackHeight(currNode->lChild) || !verifyBlackHeight(currNode->rChild)) return false; // Check children
	return true;
}

/** Recursive wrapper for verifying black height */
template <typename ElemType>
bool RedBlackTree<ElemType>::verifyBlackHeight() const {
	return verifyBlackHeight(root);
}

/** Recursive wrapper for determining if an element is contained in the tree
 * @value The value to be checked */
template <typename ElemType>
bool RedBlackTree<ElemType>::contains(const ElemType& value) const {
	return count(value) != 0;
}

/** Returns whether or not the root is black. Returns true if NULL root */
template <typename ElemType>
bool RedBlackTree<ElemType>::blackRoot() const {
	if (root == NULL) return true; // Handle NULL root
	return !root->red;	
}

/** Recursive wrapper for verifying that parent's children are children's parents */
template <typename ElemType>
bool RedBlackTree<ElemType>::parentChildMatch() const {
	return parentChildMatch(root);
}

/** Recursively checks for match between parent and child pointers
 * @currNode Node being checked */
template <typename ElemType>
bool RedBlackTree<ElemType>::parentChildMatch(Node* currNode) const {
	if (currNode == NULL) return true; // Stop at leaves
	if (currNode->lChild != NULL && currNode->lChild->parent != currNode) return false; // Check all false cases
	if (currNode->rChild != NULL && currNode->rChild->parent != currNode) return false;
	if (!parentChildMatch(currNode->lChild) || !parentChildMatch(currNode->rChild)) return false;
	return true;
}

/** Finds a given node if it exists. Returns NULL otherwise.
 * @currNode Current node being looked at
 * @value Value being searched for */
template <typename ElemType>
typename RedBlackTree<ElemType>::Node*
RedBlackTree<ElemType>::findNode(Node* currNode, const ElemType &value) const {
	if (currNode == NULL) return NULL; // If leaf, not found
	if (currNode->value == value) return currNode; //Binary search
	if (value < currNode->value) return findNode(currNode->lChild, value);
	else return findNode(currNode->rChild, value);
}

/** Returns the number of times a key is in the tree.
 * @value Value being searched for */
template <typename ElemType>
int RedBlackTree<ElemType>::count(const ElemType &value) const {
	if (findNode(root, value) == NULL) return 0;
	return findNode(root, value)->count;
}

/** Deletes an element from the tree if it exists. Otherwise, it throws an error.
 * @value Value being removed */
template <typename ElemType>
void RedBlackTree<ElemType>::remove(const ElemType &value) {
	Node* toDelete = findNode(root, value); // Check if in tree
	if (toDelete == NULL) throw std::invalid_argument("That value is not in the tree."); // Error handle
	if (toDelete->count != 1) --toDelete->count; // If count is greater than 1, no deletion necessary
	else rbDelete(toDelete); // Remove node if necessary
	--numElems; // Decrement size
}

/** Swaps the values and counts of two nodes
 * @left The first node
 * @right The second node */
template <typename ElemType>
void RedBlackTree<ElemType>::swap(Node* left, Node* right) {
	int temp = left->value;
	left->value = right->value;
	right->value = temp;
	temp = left->count;
	left->count = right->count;
	right->count = temp;
}

/** Returns the in-order predecessor of the current node.
 * @node The given node
 * @return The in-order pred. NULL if node is NULL or has no left child */
template <typename ElemType>
typename RedBlackTree<ElemType>::Node*
RedBlackTree<ElemType>::inOrderPredecessor(Node* node) const {
	if (node == NULL || node->lChild == NULL) return NULL;
	Node* inOrderPred = node->lChild;
	while (inOrderPred->rChild != NULL) inOrderPred = inOrderPred->rChild; // Go as far right as possible
	return inOrderPred;
}

/** Returns the number of NULL children of the given node. 0 if given node is NULL.
 * @node The given node */
template <typename ElemType>
int RedBlackTree<ElemType>::numNullChildren(Node* node) const {
	int num = 0;
	if (node == NULL) return num;
	if (node->lChild == NULL) num++;
	if (node->rChild == NULL) num++;
	return num;
}

/** Restores tree properties after delete.
 * @parent The parent of the node that's messing up rb properties
 * @notSibling The node that's messing up rb properties */
template <typename ElemType>
void RedBlackTree<ElemType>::deleteRestoreTree(Node* parent, Node* notSibling) {
	/** Case 0: */
	if (parent == NULL) return; // At the root. Do nothing.
	
	/** Find the sibling node */
	Node* sibling;
	bool left = false;
	if (parent->lChild != notSibling) {
		sibling = parent->lChild;
		left = true;
	}
	if (parent->rChild != notSibling) sibling = parent->rChild;

	/** Case I: Sibling is red => Parent is black */
	if (sibling->red) {
		sibling->red = parent->red; // Swap colors and rotate
		parent->red = !parent->red;
		rotate(sibling, !left);
		deleteRestoreTree(parent, notSibling); // Recurse in new position
	} else if (!parent->red && !sibling->red &&
	     	   (sibling->lChild == NULL || !sibling->lChild->red) &&
		   (sibling->rChild == NULL || !sibling->rChild->red)) {
	/** Case II: Parent is black. Sibling is black. Both sibling children are black or NULL. */
		sibling->red = true; // Just recolor.
		deleteRestoreTree(parent->parent, parent);
	} else if (parent->red && (sibling->lChild == NULL || !sibling->lChild->red)
			       && (sibling->rChild == NULL || !sibling->rChild->red)) { 
	/** Case III: Parent is red => Sibling must be black. And both sibling children must
	 * be black or NULL. */
		sibling->red = parent->red;
		parent->red = !parent->red; // Just swap colors
	} else if ((!left && (sibling->rChild == NULL || !sibling->rChild->red) && sibling->lChild->red) || ((left && (sibling->lChild == NULL || !sibling->lChild->red)) && sibling->rChild->red)) {
	/** Case IV: Inside sibling child is red. Outside sibling child is black. */
		Node* child;
		if (left) child = sibling->rChild;
		else child = sibling->lChild;
		sibling->red = child->red;
		child->red = !child->red; // Swap colors and rotate into case V.
		rotate(child, left);
		deleteRestoreTree(parent, notSibling);
	} else if ((!left && sibling->rChild != NULL && sibling->rChild->red) ||
	           (left && sibling->lChild != NULL && sibling->lChild->red)) {
	/** Case V: If the outer sibling child is red */
		bool temp;
		temp = sibling->red;
		sibling->red = parent->red;
		parent->red = temp; //Swap colors
		if (!left) sibling->rChild->red = false;
		else sibling->lChild->red = false; // Turn outer sibling child black
		rotate(sibling, !left); // Rotate
	}
}
	

/** Deals with all of the deletion cases recursively.
 * @currNode The node to be deleted */
template <typename ElemType>
void RedBlackTree<ElemType>::rbDelete(Node* currNode) {
	/** NOTE: Should only get called when currNode is non-NULL */
	int nullChildren = numNullChildren(currNode);
	switch (nullChildren) {
		/** Case 0: Node has two non-NULL children.
		 * Find io pred, swap values, and delete that */
		case 0: {
			Node* inOrderPred = inOrderPredecessor(currNode);
			swap(currNode, inOrderPred);
			rbDelete(inOrderPred);
		}
			break;
		case 1: {
			Node* child;
			if (currNode->lChild != NULL) child = currNode->lChild;
			if (currNode->rChild != NULL) child = currNode->rChild;
		/** NOTE: Parent must be black, child must be red.
		 * If parent is red, then it must have one black child, which violates
		 * black height.
		 * If parent is black and child is black, then it again violates black height.
		 */
			if (!currNode->red && child->red) {
				child->red = false;
				if (currNode->parent != NULL) {
					child->parent = currNode->parent;
					if (currNode->parent->lChild == currNode)
						currNode->parent->lChild = child;
					else currNode->parent->rChild = child;
				} else {
					root = child;
					child->parent = NULL;
				}
				delete currNode;
			}
		}
			break;
		/** Case II: Node has two NULL children.
		 * If red, remove it. If root, remove it. */	
		case 2: {
		/** Subcase II: If node is the root. */
			if (currNode == root) {
				delete root;
				root = NULL;
			} else {
		/** Subcase II: If node is red. Just replace it with NULL */
				if (currNode->parent->lChild == currNode)
					currNode->parent->lChild = NULL;
				else currNode->parent->rChild = NULL;
				//Node* sib = sibling(currNode);
				Node* parent = currNode->parent;
				bool origColor = currNode->red;
				delete currNode;
		/** Subcase III: If node is black. Delete it and restore tree props. */
				if (!origColor) deleteRestoreTree(parent, NULL);
			}
			break;	
		}
	}
}

/** Wrapper for verifying all RB Properties */
template <typename ElemType>
bool RedBlackTree<ElemType>::verifyProperties() const {
	return verifyRedChild() && parentChildMatch() && verifyBlackHeight()
	       	&& blackRoot() && verifyCount();
}

/** Helper function that copies a tree recursively
 * @from The current node to be copied from
 * @into The current node to be copied into
 * @parent The parent of the into node */
template <typename ElemType>
void RedBlackTree<ElemType>::copyTree(const Node* from, Node* &into, Node* const parent) {
	if (from == NULL) into = NULL; // Stop at leaves
	else {
		into = new Node; // Make a new node to copy into
		into->parent = parent; // Copy values
		into->value = from->value;
		into->count = from->count;
		into->red = from->red;
		copyTree(from->lChild, into->lChild, into); // Copy children
		copyTree(from->rChild, into->rChild, into);
	}
}

/** Verifies that the sum of the counts is equal to the size of the tree */
template <typename ElemType>
bool RedBlackTree<ElemType>::verifyCount() const {
	return countSum(root) == size();
}

/** Returns the sum of the counts of all nodes
 * @currNode The current node being summed */
template <typename ElemType>
int RedBlackTree<ElemType>::countSum(Node* currNode) const {
	if (currNode == NULL) return 0;
	return currNode->count + countSum(currNode->lChild) + countSum(currNode->rChild);
}

#endif // REDBLACKTREE_H
