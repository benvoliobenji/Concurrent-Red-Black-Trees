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
            // Start reading first
            parentNode->startReading();

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
                // Stop reading and move to the next node
                parentNode->stopReading();
                parentNode = childNode;
                childNode = NULL;
            }
        }
    }
}

bool RBTree::insert(int insertKey)
{
    Node* insertNode = new Node(insertKey);

    // First, check to see if this is the first node in the tree
    if (root == NULL)
    {
        insertNode->setNodeColor(Color::BLACK);
        insertNode = root;
        return true;
    }
    else
    {
        Node *parentNode = root;
        Node *childNode = NULL;

        while(true)
        {
            // Start with reading to find the leaf node to insert on
            parentNode->startReading();

            if (insertNode->getKey() > parentNode->getKey())
            {
                childNode = parentNode->getRight();
            }
            else if (insertNode->getKey <= parentNode->getKey())
            {
                childNode = parentNode->getLeft();
            }

            if (childNode == NULL)
            {
                // This is a leaf node, so lock the parent node for writing
                parentNode->stopReading();
                parentNode->startWriting();

                // Do we need to check for existence?

                // Set the parent's child
                if (insertNode->getKey() > parentNode->getKey())
                {
                    parentNode->setRight(insertNode);
                }
                else
                {
                    parentNode->setLeft(insertNode);
                }

                parentNode->stopWriting();

                // Set the child's parent
                insertNode->startWriting();
                insertNode->setParent(parentNode);
                insertNode->stopWriting();

                //TODO: Call a rebalance here to make sure the tree is balanced and doesn't have any conflicts
                return true;

            }
            else
            {
                // This is not a leaf node, so continue traversal
                Node *oldNode = parentNode;
                parentNode = childNode;
                childNode = NULL;
                oldNode->stopReading();
            }
            
        }
    }
    
}