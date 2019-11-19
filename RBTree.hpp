#pragma once

#include "Node.hpp"

class RBTree
{
    private:
        Node *root;

    public:
        RBTree() { root = NULL; };

        ~RBTree() { delete(root); }
        
        bool search(int searchKey);

        bool insert(int insertKey);

        bool insertNode(Node* insertNode);

        bool deleteNode(int deleteKey);
};
