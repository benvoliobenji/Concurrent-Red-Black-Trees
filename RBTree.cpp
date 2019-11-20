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

                fixInsertionViolation(newNode);
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
    parentNode->stopWriting();
    Color originalColor = childNode->getNodeColor();
    Node *yNode = childNode;
    Node *xNode = NULL;

    if (childNode->getLeft() == NULL)
    {
        xNode = childNode->getRight();
        xNode->startWriting();

        transplant(childNode, childNode->getRight());
    }
    else if (childNode->getRight() == NULL)
    {
        xNode = childNode->getLeft();
        xNode->startWriting();

        transplant(childNode, childNode->getLeft());
    }
    else
    {
        yNode = minimum(childNode->getRight());
        yNode->startWriting();

        originalColor = yNode->getNodeColor();

        xNode = yNode->getRight();
        xNode->startWriting();

        if (yNode->getParent() == childNode)
        {
            xNode->setParent(yNode);
        }
        else
        {
            transplant(yNode, yNode->getRight());
            yNode->setRight(childNode->getRight());

            yNode->getRight()->startWriting();
            yNode->getRight()->setParent(yNode);
            yNode->getRight()->stopWriting();
        }

        transplant(childNode, yNode);
        yNode->setLeft(childNode->getLeft());

        yNode->getLeft()->startWriting();
        yNode->getLeft()->setParent(yNode);
        yNode->getLeft()->stopWriting();

        yNode->setNodeColor(childNode->getNodeColor());

        yNode->stopWriting();
    }

    xNode->stopWriting();
    childNode->stopWriting();
    delete(childNode);

    if (originalColor == Color::BLACK)
    {
        // TODO: Fix deletion violation code implementation
        fixDeletionViolation(xNode);
    }
}

void RBTree::fixInsertionViolation(Node *insertionNode)
{
    Node *violationNode = insertionNode;
    violationNode->startWriting();


    Node *parent = NULL;
    Node *grandParent = NULL;

    parent = violationNode->getParent();
    
    parent->startWriting();

    while ((violationNode != root) && (violationNode->getNodeColor() == Color::BLACK) && (parent->getNodeColor() == Color::RED))
    {
        if (parent != NULL && parent != violationNode)
        {
            parent->stopWriting();
        }
        if (grandParent != NULL && grandParent != violationNode)
        {
            grandParent->stopWriting();
        }

        parent = violationNode->getParent();
        parent->startWriting();

        grandParent = parent->getParent();
        grandParent->startWriting();

        // 1st Case: Parent of the violation node is the left child of grandparent
        if (parent == grandParent->getLeft())
        {
            Node *uncle = grandParent->getRight();

            if (uncle != NULL)
            {
                uncle->startWriting();

                // Case 1A: Uncle of violation node is also red
                // Only need to recolor
                if (uncle->getNodeColor() == Color::RED)
                {
                    grandParent->setNodeColor(Color::RED);
                    parent->setNodeColor(Color::BLACK);
                    uncle->setNodeColor(Color::BLACK);

                    violationNode->stopWriting();
                    violationNode = grandParent;
                }

                uncle->stopWriting();
            }
            else
            {
                // Case 1B: violation node is the right child of its parent
                // Now we need to do a left-rotation
                if (violationNode == parent->getRight())
                {
                    violationNode->stopWriting();
                    parent->stopWriting();
                    grandParent->stopWriting();

                    rotateLeft(parent);

                    parent->startWriting();
                    grandParent->startWriting();

                    violationNode = parent;
                    parent = parent->getParent();
                }
                else
                {
                    // Case 1C: violation node is the left child of its parent
                    // Now we need to do a right-rotation
                    violationNode->stopWriting();
                    parent->stopWriting();
                    grandParent->stopWriting();

                    rotateRight(grandParent);

                    parent->startWriting();
                    grandParent->startWriting();

                    // Swap the colors
                    Color parentColor = parent->getNodeColor();
                    parent->setNodeColor(grandParent->getNodeColor());
                    grandParent->setNodeColor(parentColor);

                    violationNode = parent;
                }   
            }
        }
        // Case 2: parent is the right child of  grandParent
        else
        {
            Node *uncle = grandParent->getLeft();

            if (uncle != NULL)
            {
                uncle->startWriting();

                // Case 2A: Uncle is also red
                // Only need to recolor
                if (uncle->getNodeColor() == Color::RED)
                {
                    grandParent->setNodeColor(Color::RED);
                    parent->setNodeColor(Color::BLACK);
                    uncle->setNodeColor(Color::BLACK);

                    violationNode->stopWriting();
                    violationNode = grandParent;
                }

                uncle->stopWriting();
            }
            else
            {
                // Case 2B: violationNode is left child of parent
                // Now we do a right-rotation
                if (violationNode == parent->getLeft())
                {
                    violationNode->stopWriting();
                    parent->stopWriting();
                    grandParent->stopWriting();

                    rotateRight(parent);

                    parent->startWriting();
                    grandParent->startWriting();

                    violationNode = parent;
                    parent = parent->getParent();
                }
                else
                {
                    // Case 2C: violationNode is right child of parent
                    // Now we do a left-rotation
                    violationNode->stopWriting();
                    parent->stopWriting();
                    grandParent->stopWriting();

                    rotateLeft(grandParent);

                    parent->startWriting();
                    grandParent->startWriting();

                    // Swap the colors
                    Color parentColor = parent->getNodeColor();
                    parent->setNodeColor(grandParent->getNodeColor());
                    grandParent->setNodeColor(parentColor);

                    violationNode = parent;
                }    
            }
        }      
    }

    // Make sure we stop writing
    violationNode->stopWriting();

    if (parent != NULL && parent != violationNode)
    {
        parent->stopWriting();
    }
    if (grandParent != NULL && grandParent != violationNode && grandParent != parent)
    {
        grandParent->stopWriting();
    }

    // Make sure we set the root to black
    root->startWriting();
    root->setNodeColor(Color::BLACK);
}

void RBTree::rotateLeft(Node *rotateNode)
{
    rotateNode->startWriting();
    Node *rotateNodeRight = rotateNode->getRight();

    rotateNodeRight->startWriting();

    rotateNode->setRight(rotateNodeRight->getLeft());

    if (rotateNode->getRight() != NULL)
    {
        rotateNode->getRight()->startWriting();
        rotateNode->getRight()->setParent(rotateNode);
        rotateNode->getRight()->stopWriting();
    }

    rotateNodeRight->setParent(rotateNode->getParent());

    if (rotateNode->getParent() == NULL)
    {
        root = rotateNodeRight;
    }
    else
    {
        Node *rotateNodeParent = rotateNode->getParent();
        rotateNodeParent->startWriting();

        if (rotateNode == rotateNodeParent->getLeft())
        {
            rotateNodeParent->setLeft(rotateNodeRight);
        }
        else
        {
            rotateNodeParent->setRight(rotateNodeRight);
        }

        rotateNodeParent->stopWriting();
    }

    rotateNodeRight->setLeft(rotateNode);
    rotateNode->setParent(rotateNodeRight);

    rotateNodeRight->stopWriting();
    rotateNode->stopWriting();
}

void RBTree::rotateRight(Node *rotateNode)
{
    rotateNode->startWriting();

    Node *rotateNodeLeft = rotateNode->getLeft();
    rotateNodeLeft->startWriting();

    rotateNode->setLeft(rotateNodeLeft->getRight());

    if (rotateNode->getLeft() != NULL)
    {
        rotateNode->getLeft()->startWriting();
        rotateNode->getLeft()->setParent(rotateNode);
        rotateNode->getLeft()->stopWriting();
    }

    rotateNodeLeft->setParent(rotateNode->getParent());

    if (rotateNode->getParent() == NULL)
    {
        root = rotateNodeLeft;
    }
    else
    {
        Node *rotateNodeParent = rotateNode->getParent();
        rotateNodeParent->startWriting();

        if (rotateNode == rotateNodeParent->getLeft())
        {
            rotateNodeParent->setLeft(rotateNodeLeft);
        }
        else
        {
            rotateNodeParent->setRight(rotateNodeLeft);
        }
        
        rotateNodeParent->stopWriting();
    }

    rotateNodeLeft->setRight(rotateNode);
    rotateNode->setParent(rotateNodeLeft);

    rotateNodeLeft->stopWriting();
    rotateNode->stopWriting();
}


void RBTree::transplant(Node *removedNode, Node *newNode)
{
    if (removedNode->getParent() == NULL)
    {
        root = newNode;
    }
    else if (removedNode == removedNode->getParent()->getLeft())
    {
        removedNode->getParent()->setLeft(newNode);
    }
    else
    {
        removedNode->getParent()->setRight(newNode);
    }

    newNode->setParent(removedNode->getParent());
}

Node *RBTree::minimum(Node *node)
{
    node->startReading();
    Node *leftNode = NULL;

    while (node->getLeft() != NULL)
    {
        leftNode = node->getLeft();
        leftNode->startReading();
        node->stopReading();
        node = leftNode;
    }

    node->stopReading();
    return node;
}