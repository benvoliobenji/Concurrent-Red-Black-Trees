#pragma once

#include "Node.hpp"

class RBTree
{
    private:
        Node *root;

    public:
        bool search(int searchKey);

        bool insert(int insertKey);

        bool deleteNode(int deleteKey);
};
