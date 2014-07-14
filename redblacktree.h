#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H
#define NULL 0

template <typename ElemType>
class RedBlackTree {
public:
    RedBlackTree();
    /** Destructor */
    ~RedBlackTree();
    
    /**
     * Inserts an element
     */
    void insert(ElemType value);

    /** Returns the number of elements in the tree */
    int size();

    /** Returns if the tree is empty or not */
    bool empty();
private:
    typedef struct Node {
	    ElemType value;
	    bool red;
	    Node* lChild;
	    Node* rChild;
    } Node;

    Node* root;
    int numElems;

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

}

template <typename ElemType>
void RedBlackTree<ElemType>::insert(ElemType value) {

}

template <typename ElemType>
int RedBlackTree<ElemType>::size() {
	return numElems;
}

template <typename ElemType>
bool RedBlackTree<ElemType>::empty() {
	return size() == 0;
}


#endif // REDBLACKTREE_H
