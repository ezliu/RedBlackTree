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

    /** Checks if the root is black */
    bool blackRoot() const;

    /** Checks if parents and children pointers match */
    bool parentChildMatch() const;
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

    /** Fixes any errors in coloring from insertion/deletion */
    void restoreTree(Node* child);

    /** For debugging purposes: verifies that red nodes have only black children */
    bool verifyRedChild(Node* currNode) const;

    /** For debugging purposes: returns the blackheight of a given node recursively */
    //TODO: Use memoization
    int blackHeight(Node* currNode, const bool left) const;

    /** Recursive function to check all blackheights */
    bool verifyBlackHeight(Node* currNode) const;

    /** Recursively checks if a value is contained */
    bool contains(Node* currNode, const ElemType& value) const;

    /** Recursively checks that parent and children pointers match */
    bool parentChildMatch(Node* currNode) const;
};

/**
 * Implementation details */

template <typename ElemType>
RedBlackTree<ElemType>::RedBlackTree():
	root(NULL),
	numElems(0)
{}

template <typename ElemType>
RedBlackTree<ElemType>::~RedBlackTree() {
	recursiveDelete(root);
}

template <typename ElemType>
void RedBlackTree<ElemType>::insert(const ElemType &value) {
	//std::cout << "Inserting: " << value <<  std::endl;
	numElems++;
	recursiveInsert(value, root);
}

template <typename ElemType>
int RedBlackTree<ElemType>::size() const {
	return numElems;
}

template <typename ElemType>
bool RedBlackTree<ElemType>::empty() const {
	return size() == 0;
}

//template <typename ElemType>
//std::stringstream& RedBlackTree<ElemType>::debugString() const {
//	std::stringstream debugString;
//	std::queue<Node*> myQueue;
//	}
//	myQueue.push(root);
//	std::queue<Node*> nextQueue;
//	while (true) {
//		bool allNull = true;
//		Node* nextNode = myQueue.front();
//		myQueue.pop();
//		if (nextNode == NULL) {
//			nextQueue.push(NULL);
//			nextQueue.push(NULL);
//			debugString << "NULL b ";
//		} else {
//			nextQueue.push(nextNode->lChild);
//			nextQueue.push(nextNode->rChild);
//			debugString << nextNode->value;
//			debugString << " " << (nextNode->red) ? "r  " : "b ";
//			allNull = false;
//		}
//		if (myQueue.empty()) {
//			debugString << "\n";
//			if (allNull) return debugString;
//			std::swap(myQueue, nextQueue);
//		}
//	}
//}

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

template <typename ElemType>
void RedBlackTree<ElemType>::recursiveInsert(const ElemType& value, Node* currNode) {
	//std::cout<<"Recursive Inserting: " << value << std::endl;
	//if (currNode != NULL) std::cout << "Curr Node: " << currNode->value << std::endl;
	//else std::cout << "Curr Node: NULL" << std::endl;

	if (root == NULL) { // Insert root node
		root = makeNode(value, NULL, false);
		return;
	}
	if (value < currNode->value) { // Traverse left
		//std::cout << "Traversing left!" << std::endl;
		if (currNode->lChild == NULL) { // If insertable, place node.
			//std::cout << "Inserting left!" << std::endl;
			currNode->lChild = makeNode(value, currNode);
			restoreTree(currNode->lChild);
		} else {
			//std::cout << currNode->lChild << std::endl;
			recursiveInsert(value, currNode->lChild);
		}	
	} else if (value == currNode->value) {
		//std::cout << "Middle" << std::endl;
	       	currNode->count++; // Duplicates insert
		//std::cout << "We're still alive." << std::endl;
	} else { // Traverse right
		//std::cout << "Traversing right!" << std::endl;
		if (currNode->rChild == NULL) { // If insertable, place node.
			//std::cout << "Inserting right!" << std::endl;
			currNode->rChild = makeNode(value, currNode);
			restoreTree(currNode->rChild);
		} else recursiveInsert(value, currNode->rChild);
	}
}

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

template <typename ElemType>
void RedBlackTree<ElemType>::recursiveDelete(Node*& currNode) {
	if (currNode == NULL) return;
	recursiveDelete(currNode->lChild);
	recursiveDelete(currNode->rChild);
	delete currNode;
}

template <typename ElemType>
void RedBlackTree<ElemType>::rotate(Node* child, const bool left) {
	Node* origParent = child->parent; // Store original pointers
	Node* origGrandparent = origParent->parent;
	child->parent = origGrandparent;
	//std::cout << "Child: " << child->value << std::endl;
//	if (origParent != NULL) std::cout << "Parent: " << origParent->value << std::endl;
//	if (origGrandparent != NULL) std::cout << "Grandparent: " << origGrandparent->value << std::endl;
	//std::cout << "Parent: " << ((origParent == NULL) ? 0 : origParent->value) << std::endl;
	//std::cout << "Grandparent: " << ((origGrandparent == NULL) ? 0 : origGrandparent->value) << std::endl;
	origParent->parent = child;
	if (left) { // Left rotate 
		Node* origLGrandchild = child->lChild;
		//std::cout << "Orig L Grandchild: " << origLGrandchild << std::endl;
		child->lChild = origParent;
		origParent->rChild = origLGrandchild;
		if (origLGrandchild != NULL) origLGrandchild->parent = origParent;
		//std::cout << "Child: " << child << std::endl;
	//	std::cout << "Child: " << child->value << std::endl;
		//std::cout << "Child Parent: " << child->parent->value << std::endl;
	//	std::cout << "Child lChild: " << child->lChild->value << std::endl;
	//	std::cout << "Child rChild: " << child->rChild << std::endl;
	//	std::cout << "Orig Parent: " << origParent->value << std::endl;
	//	std::cout << "Orig Parent Parent: " << origParent->parent->value << std::endl;
	//	std::cout << "Orig Parent lChild: " << origParent->lChild << std::endl;
	//	std::cout << "Orig Parent rChild: " << origParent->rChild->value << std::endl;
	/** TODO: Implement RIGHT and LEFT "grandparents" */
	} else { // Right rotate
		Node* origRGrandchild = child->rChild;
		child->rChild = origParent;
		origParent->lChild = origRGrandchild;
		if (origRGrandchild != NULL) origRGrandchild->parent = origParent;
	}
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

/** Gets called to restore red black properties */
template <typename ElemType>
void RedBlackTree<ElemType>::restoreTree(Node* child) {
	// NOTE: This should not get called on a NULL node, so child should never be NULL.
	if (child == NULL) std::cout<< "NOOOO!" << std::endl;
	if (root->red == true) {
		//std::cout << "Case I: Redo Root Color.\n";
		root->red = false;
		return;
	}
	if (child->parent == NULL) { // If node is root, recolor. 
		//std::cout << "HELP!" << std::endl;
		child->red = false;
		return;
	}
	if (child->red && !child->parent->red) {
		//std::cout << "Case II: Reg Insert. \n";
		return; // Case I. Regular insertion.
	}
	if (child->red && child->parent->red) {
		if (uncle(child) != NULL && uncle(child)->red) { // Case II. Parent && Uncle are red.
			//std::cout << "Case III: Parent Grandparent Color Swap. \n";
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
				//std::cout << "Case IV: Parent and Child are on diff sides -left. \n";
				Node* origParent = child->parent;
				rotate(child, true); // Left rotate
				restoreTree(origParent); // Proceed to Case IV with new child.
			} else if (grandparent(child)->rChild == child->parent && child == child->parent->lChild) {
				//std::cout << "Case IV: Parent and Child are on diff sides -right. \n";
				Node* origParent = child->parent;
				rotate(child, false); // Right rotate
				restoreTree(origParent); // Case IV
			} else { // Case IV: Parent and Child are on same side.
				/** Solution: Rotate parent and grandparent and then swap colors */
				//std::cout << "Case V: Parent and Child on same sides. \n";
				Node* origParent = child->parent;
				Node* origGrandparent = grandparent(child);
				origParent->red = origGrandparent->red;
				origGrandparent->red = !origGrandparent->red;
				if (origParent == origGrandparent->lChild) rotate(origParent, false);
				else rotate(origParent, true);
				//root->red = false; // Make sure root stays black
				//restoreTree(origParent);
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
	if (blackHeight(currNode, true) != blackHeight(currNode, false)) {
		std::cout << "Curr Node: " << currNode->value << std::endl;
		std::cout << "Left Black Height: " << blackHeight(currNode, true) << std::endl;
		std::cout << "Right Black Height: " << blackHeight(currNode, false) << std::endl;
		return false;
	}
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
	return contains(root, value);
}

/** Recursively determines if tree contains given element */
template <typename ElemType>
bool RedBlackTree<ElemType>::contains(Node* currNode, const ElemType& value) const {
	if (currNode == NULL) return false;
	if (currNode->value == value) return true;
	if (value < currNode->value) return contains(currNode->lChild, value);
	else return contains(currNode->rChild, value);
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
	if (currNode->lChild != NULL && currNode->lChild->parent != currNode) {
		std::cout << "Curr Node: " << currNode->value << std::endl;
		std::cout << "Left Child: " << currNode->lChild->value << std::endl;
		std::cout << "Left Child Parent: " << currNode->lChild->parent->value << std::endl;
		print();
		return false;
	}
	if (currNode->rChild != NULL && currNode->rChild->parent != currNode) {
		std::cout << "Curr Node: " << currNode->value << std::endl;
		std::cout << "Left Child: " << currNode->lChild->value << std::endl;
		std::cout << "Left Child Parent: " << currNode->lChild->parent->value << std::endl;
		print();
		return false;
	}
	if (!parentChildMatch(currNode->lChild) || !parentChildMatch(currNode->rChild)) return false;
	return true;
}
#endif // REDBLACKTREE_H
