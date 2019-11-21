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

        // Start reading first
        startReadingNode(parentNode);

        while (true)
        {
            // Check on the key
            if (parentNode->getKey() == searchKey)
            {
                stopReadingNode(parentNode);
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
                startReadingNode(childNode);
                stopReadingNode(parentNode);
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
    // This is the base case
    if (root == NULL)
    {
        startWritingNode(newNode);
        newNode->setNodeColor(Color::BLACK);
        root = newNode;
        stopWritingNode(newNode);
        return true;
    }
    // Else, start traversing to find the best spot to put the node
    else
    {
        Node *parentNode = root;
        Node *childNode = NULL;

        // Start with reading to find the leaf node to insert on
        // Using writing condition variable to enforce execution order
        startWritingNode(parentNode);

        while(true)
        {
            // Find an appropriate child node based on the key
            if (newNode->getKey() > parentNode->getKey())
            {
                childNode = parentNode->getRight();
            }
            else if (newNode->getKey() <= parentNode->getKey())
            {
                childNode = parentNode->getLeft();
            }

            // If the child is null, then we've hit a spot to insert
            if (childNode == NULL)
            {
                // Set the parent's child
                if (newNode->getKey() > parentNode->getKey())
                {
                    parentNode->setRight(newNode);
                }
                else
                {
                    parentNode->setLeft(newNode);
                }

                stopWritingNode(parentNode);

                // Set the child's parent
                startWritingNode(newNode);
                newNode->setParent(parentNode);
                stopWritingNode(newNode);

                // The child is inserted, now check for potential violations
                fixInsertionViolation(newNode);
                return true;

            }
            else
            {
                // This is not a leaf node, so continue traversal
                startWritingNode(childNode);
                stopWritingNode(parentNode);
                parentNode = childNode;
                childNode = NULL;
            }
        }
    }
}

bool RBTree::deleteNode(int deleteKey)
{
    Node *parentNode = root;
    Node *childNode = NULL;

    startWritingNode(parentNode);

    // Start the search for the node to delete
    while (true)
    {
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
            stopWritingNode(parentNode);
            return false;
        }
        else if (childNode->getKey() == deleteKey)
        {
            // We've found the node to delete, so lock it and get ready for deletion
            startWritingNode(childNode);
            break;
        }
        else
        {
            // This is not a node we need to delete, so keep moving
            startWritingNode(childNode);
            stopWritingNode(parentNode);
            parentNode = childNode;
            childNode = NULL;
        }
    }

    // Time to delete the node
    stopWritingNode(parentNode);

    // First, get the original color of the node to delete
    Color originalColor = childNode->getNodeColor();

    Node *yNode = childNode;
    Node *xNode = NULL;

    if (childNode->getLeft() == NULL)
    {
        xNode = childNode->getRight();
        startWritingNode(xNode);

        // Transplant the child with the child's right node
        transplant(childNode, childNode->getRight());
    }
    else if (childNode->getRight() == NULL)
    {
        xNode = childNode->getLeft();
        startWritingNode(xNode);

        // Transplant the child with the child's left node
        transplant(childNode, childNode->getLeft());
    }
    else
    {
        // Find the minimum value in the child's right tree
        yNode = minimum(childNode->getRight());
        startWritingNode(yNode);

        // Change the original color to yNode
        originalColor = yNode->getNodeColor();

        // Get the right children of the yNode
        xNode = yNode->getRight();
        startWritingNode(xNode);

        if (yNode->getParent() == childNode)
        {
            xNode->setParent(yNode);
        }
        else
        {
            transplant(yNode, yNode->getRight());
            yNode->setRight(childNode->getRight());

            startWritingNode(yNode->getRight());
            yNode->getRight()->setParent(yNode);
            stopWritingNode(yNode->getRight());
        }

        transplant(childNode, yNode);
        yNode->setLeft(childNode->getLeft());

        startWritingNode(yNode->getLeft());
        yNode->getLeft()->setParent(yNode);
        stopWritingNode(yNode->getLeft());

        yNode->setNodeColor(childNode->getNodeColor());

        stopWritingNode(yNode);
    }

    stopWritingNode(xNode);
    stopWritingNode(childNode);
    delete(childNode);

    // We only need to fix violations if the original color of the transplanted node was black
    if (originalColor == Color::BLACK)
    {
        fixDeletionViolation(xNode);
    }
}

void RBTree::fixDeletionViolation(Node *deletionNode)
{
    Node *sNode;
    Node *violationNode = deletionNode;

    startWritingNode(violationNode);

    while (violationNode != root && violationNode->getNodeColor() == Color::BLACK)
    {
        Node *parent = violationNode->getParent();
        startWritingNode(parent);

        if (violationNode == parent->getLeft())
        {
            sNode = parent->getRight();
            startWritingNode(sNode);

            if (sNode->getNodeColor() == Color::RED)
            {
                sNode->setNodeColor(Color::BLACK);
                parent->setNodeColor(Color::RED);

                stopWritingNode(sNode);
                stopWritingNode(parent);
                rotateLeft(parent);

                startWritingNode(sNode);
                parent = sNode->getParent();
                startWritingNode(parent);

                stopWritingNode(sNode);
                sNode = parent->getRight();
                stopWritingNode(parent);
                startWritingNode(sNode);
            }

            Node *sLeftNode = sNode->getLeft();
            Node *sRightNode = sNode->getRight();

            startWritingNode(sLeftNode);
            startWritingNode(sRightNode);
            if (sLeftNode->getNodeColor() == Color::BLACK && sRightNode->getNodeColor() == Color::BLACK)
            {
                sNode->setNodeColor(Color::RED);
                Node *tempNode = violationNode->getParent();
                stopWritingNode(violationNode);
                violationNode = tempNode;
                startWritingNode(violationNode);

                // Now that violationNode has changed, update the parent as well
                stopWritingNode(parent);
                parent = violationNode->getParent();
                startWritingNode(parent);

                stopWritingNode(sLeftNode);
                stopWritingNode(sRightNode);
            }
            else
            {
                if(sRightNode->getNodeColor() == Color::BLACK)
                {
                    sLeftNode->setNodeColor(Color::BLACK);
                    sNode->setNodeColor(Color::RED);

                    stopWritingNode(sLeftNode);
                    stopWritingNode(sRightNode);
                    stopWritingNode(sNode);

                    rotateRight(sNode);

                    sNode = parent->getRight();

                    startWritingNode(sNode);
                    sLeftNode = sNode->getLeft();
                    sRightNode = sNode->getRight();
                    startWritingNode(sLeftNode);
                    startWritingNode(sRightNode);
                }

                stopWritingNode(sLeftNode);
                stopWritingNode(sRightNode);

                sNode->setNodeColor(parent->getNodeColor());
                parent->setNodeColor(Color::BLACK);

                sRightNode = sNode->getRight();
                startWritingNode(sRightNode);
                sRightNode->setNodeColor(Color::BLACK);
                stopWritingNode(sRightNode);

                stopWritingNode(sNode);
                stopWritingNode(violationNode);
                stopWritingNode(parent);

                rotateLeft(parent);

                violationNode = root;
                startWritingNode(violationNode);
            }
        }
        else
        {
            sNode = parent->getLeft();
            startWritingNode(sNode);

            if (sNode->getNodeColor() == Color::RED)
            {
                sNode->setNodeColor(Color::BLACK);
                parent->setNodeColor(Color::RED);

                stopWritingNode(sNode);
                stopWritingNode(violationNode);
                stopWritingNode(parent);

                rotateRight(parent);

                startWritingNode(violationNode);
                parent = violationNode->getParent();
                startWritingNode(parent);

                sNode = parent->getLeft();
                startWritingNode(sNode);
            }

            Node *sRightNode = sNode->getRight();
            Node *sLeftNode = sNode->getLeft();

            startWritingNode(sRightNode);
            startWritingNode(sLeftNode);

            if (sRightNode->getNodeColor() == Color::BLACK && sLeftNode->getNodeColor() == Color::BLACK)
            {
                sNode->setNodeColor(Color::RED);
                stopWritingNode(violationNode);
                violationNode = parent;
                parent = violationNode->getParent();
                startWritingNode(parent);

                stopWritingNode(sRightNode);
                stopWritingNode(sLeftNode);
            }
            else
            {
                if (sLeftNode->getNodeColor() == Color::BLACK)
                {
                    sRightNode->setNodeColor(Color::BLACK);
                    sNode->setNodeColor(Color::RED);

                    stopWritingNode(sLeftNode);
                    stopWritingNode(sRightNode);
                    stopWritingNode(sNode);

                    rotateLeft(sNode);

                    sNode = parent->getLeft();
                    startWritingNode(sNode);

                    sLeftNode = sNode->getLeft();
                    sRightNode = sNode->getRight();

                    startWritingNode(sLeftNode);
                    startWritingNode(sRightNode);
                }

                sNode->setNodeColor(parent->getNodeColor());
                parent->setNodeColor(Color::BLACK);
                sLeftNode->setNodeColor(Color::BLACK);

                stopWritingNode(sLeftNode);
                stopWritingNode(sRightNode);
                stopWritingNode(sNode);
                stopWritingNode(violationNode);
                stopWritingNode(parent);

                rotateRight(parent);

                violationNode = root;
                startWritingNode(parent);
            }
        }
    }

    violationNode->setNodeColor(Color::BLACK);
    stopWritingNode(violationNode);
}

void RBTree::fixInsertionViolation(Node *insertionNode)
{
    Node *violationNode = insertionNode;
    startWritingNode(violationNode);

    Node *parent = NULL;
    Node *grandParent = NULL;

    parent = violationNode->getParent();  
    startWritingNode(parent);

    while ((violationNode != root) && (violationNode->getNodeColor() == Color::BLACK) && (parent->getNodeColor() == Color::RED))
    {
        if (parent != NULL && parent != violationNode)
        {
            stopWritingNode(parent);
        }
        if (grandParent != NULL && grandParent != violationNode)
        {
            stopWritingNode(grandParent);
        }

        parent = violationNode->getParent();
        startWritingNode(parent);

        grandParent = parent->getParent();
        startWritingNode(grandParent);

        // 1st Case: Parent of the violation node is the left child of grandparent
        if (parent == grandParent->getLeft())
        {
            Node *uncle = grandParent->getRight();

            if (uncle != NULL)
            {
                startWritingNode(uncle);

                // Case 1A: Uncle of violation node is also red
                // Only need to recolor
                if (uncle->getNodeColor() == Color::RED)
                {
                    grandParent->setNodeColor(Color::RED);
                    parent->setNodeColor(Color::BLACK);
                    uncle->setNodeColor(Color::BLACK);

                    stopWritingNode(violationNode);
                    violationNode = grandParent;
                }

                stopWritingNode(uncle);
            }
            else
            {
                // Case 1B: violation node is the right child of its parent
                // Now we need to do a left-rotation
                if (violationNode == parent->getRight())
                {
                    stopWritingNode(violationNode);
                    stopWritingNode(parent);
                    stopWritingNode(grandParent);

                    rotateLeft(parent);

                    startWritingNode(parent);
                    startWritingNode(grandParent);

                    violationNode = parent;
                    parent = parent->getParent();
                }
                else
                {
                    // Case 1C: violation node is the left child of its parent
                    // Now we need to do a right-rotation
                    stopWritingNode(violationNode);
                    stopWritingNode(parent);
                    stopWritingNode(grandParent);

                    rotateRight(grandParent);

                    startWritingNode(parent);
                    startWritingNode(grandParent);

                    // Swap the colors
                    Color parentColor = parent->getNodeColor();
                    parent->setNodeColor(grandParent->getNodeColor());
                    grandParent->setNodeColor(parentColor);

                    violationNode = parent;
                }   
            }
        }
        // Case 2: parent is the right child of grandParent
        else
        {
            Node *uncle = grandParent->getLeft();

            if (uncle != NULL)
            {
                startWritingNode(uncle);

                // Case 2A: Uncle is also red
                // Only need to recolor
                if (uncle->getNodeColor() == Color::RED)
                {
                    grandParent->setNodeColor(Color::RED);
                    parent->setNodeColor(Color::BLACK);
                    uncle->setNodeColor(Color::BLACK);

                    stopWritingNode(violationNode);
                    violationNode = grandParent;
                }

                stopWritingNode(uncle);
            }
            else
            {
                // Case 2B: violationNode is left child of parent
                // Now we do a right-rotation
                if (violationNode == parent->getLeft())
                {
                    stopWritingNode(violationNode);
                    stopWritingNode(parent);
                    stopWritingNode(grandParent);

                    rotateRight(parent);

                    startWritingNode(parent);
                    startWritingNode(grandParent);

                    violationNode = parent;
                    parent = parent->getParent();
                }
                else
                {
                    // Case 2C: violationNode is right child of parent
                    // Now we do a left-rotation
                    stopWritingNode(violationNode);
                    stopWritingNode(parent);
                    stopWritingNode(grandParent);

                    rotateLeft(grandParent);

                    stopWritingNode(parent);
                    stopWritingNode(grandParent);

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
    stopWritingNode(violationNode);

    if (parent != NULL && parent != violationNode)
    {
        stopWritingNode(parent);
    }
    if (grandParent != NULL && grandParent != violationNode && grandParent != parent)
    {
        stopWritingNode(grandParent);
    }

    // Make sure we set the root to black
    startWritingNode(root);
    root->setNodeColor(Color::BLACK);
}

void RBTree::rotateLeft(Node *rotateNode)
{
    startWritingNode(rotateNode);
    Node *rotateNodeRight = rotateNode->getRight();

    startWritingNode(rotateNodeRight);

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