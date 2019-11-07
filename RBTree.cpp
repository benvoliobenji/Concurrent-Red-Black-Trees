#include "RBTree.hpp"

bool RBTree::search(int searchKey) 
{
    // Check if a tree even exists first
    if (root == NULL)
    {
        return false;
    }
    else
    {
        Node *parentNode = root;
        Node *childNode = NULL;
        while (true)
        {
            // Lock the node first
            parentNode->lockNode();

            // Check on the key
            if (parentNode->getKey() == searchKey)
            {
                return true;
            }
            else if (parentNode->getKey() < searchKey)
            {
                childNode = parentNode->getRight();
            }
            else if (parentNode->getKey() > searchKey)
            {
                childNode = parentNode->getLeft();
            }

            if (childNode == NULL)
            {
                return false;
            }
            else
            {
                parentNode->unlockNode();
                parentNode = childNode;
                childNode = NULL;
            }
        }
    }
}