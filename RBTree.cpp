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
    Node *newNode = new Node(insertKey);
    return insertNode(newNode);
    // Actually call the reorg of the tree here rather than in the helper function
}

bool RBTree::insertNode(Node* newNode)
{
    // First, check to see if this is the first node in the tree
    if (root == NULL)
    {
        newNode->setNodeColor(Color::BLACK);
        newNode = root;
        return true;
    }
    else
    {
        Node *parentNode = root;
        Node *childNode = NULL;

        while(true)
        {
            // Start with reading to find the leaf node to insert on
            // Using writing condition variable to enforce execution order
            parentNode->startWriting();

            if (newNode->getKey() > parentNode->getKey())
            {
                childNode = parentNode->getRight();
            }
            else if (newNode->getKey() <= parentNode->getKey())
            {
                childNode = parentNode->getLeft();
            }

            if (childNode == NULL)
            {
                // Do we need to check for existence?

                // Set the parent's child
                if (newNode->getKey() > parentNode->getKey())
                {
                    parentNode->setRight(newNode);
                }
                else
                {
                    parentNode->setLeft(newNode);
                }

                parentNode->stopWriting();

                // Set the child's parent
                newNode->startWriting();
                newNode->setParent(parentNode);
                newNode->stopWriting();

                //TODO: Call a rebalance here to make sure the tree is balanced and doesn't have any conflicts
                return true;

            }
            else
            {
                // This is not a leaf node, so continue traversal
                Node *oldNode = parentNode;
                parentNode = childNode;
                childNode = NULL;
                oldNode->stopWriting();
            }
            
        }
    }
}

bool RBTree::deleteNode(int deleteKey)
{
    Node *parentNode = root;
    Node *childNode = NULL;

    while (true)
    {
        // Start the process of finding the node to delete
        parentNode->startWriting();

        if (deleteKey < parentNode->getKey())
        {
            childNode = parentNode->getLeft();
        }
        else if (deleteKey > parentNode->getKey())
        {
            childNode = parentNode->getRight();
        }

        if (childNode == NULL)
        {
            // We couldn't find the node we needed to delete, so just return false
            parentNode->stopWriting();
            return false;
        }
        else if (childNode->getKey() == deleteKey)
        {
            // We've found the node to delete, so lock it and get ready for deletion
            childNode->startWriting();
            break;
        }
        else
        {
            // This is not a node we need to delete, so keep moving
            Node *oldNode = parentNode;
            parentNode = childNode;
            childNode = NULL;
            oldNode->stopWriting();
        }
    }

    // Time to delete the node
    Node *newChildNode;
    if (childNode->getKey() < parentNode->getKey())
    {
    }
}