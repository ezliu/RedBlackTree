#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

template <typename ElemType>
class RedBlackTree {
public:
    RedBlackTree();
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



#endif // REDBLACKTREE_H
