#pragma once

#include "Node.hpp"

class RBTree
{
    private:
        Node *root;

        void fixInsertionViolation(Node *insertionNode);

        void rotateLeft(Node *rotateNode);

        void rotateRight(Node *rotateNode);

        void transplant(Node *removedNode, Node *newNode);

        Node *minimum(Node *node);

    public:
        RBTree() { root = NULL; };

        ~RBTree() { delete(root); }
        
        bool search(int searchKey);

        bool insert(int insertKey);

        bool insertNode(Node* insertNode);

        bool deleteNode(int deleteKey);
};
