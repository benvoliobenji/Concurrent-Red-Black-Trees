#include "RBTree.hpp"
#include <iostream>

void RBTree::print()
{
    printREC(root, 0);
    std::cout << std::endl;
}

void RBTree::printREC(Node *printNode, int level)
{
    if (printNode != NULL)
    {
        startReadingNode(printNode);
        std::string color = (printNode->getNodeColor() == Color::BLACK) ? "b" : "r";
        std::cout << printNode->getKey() << color << " ";
        printREC(printNode->getLeft(), level + 1);
        printREC(printNode->getRight(), level + 1);
        stopReadingNode(printNode);
    }
    else
    {
        std::cout << "f ";
    }
}

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
    // std::cout << "Inserting node " << newNode->getKey() << std::endl;

    // First, check to see if this is the first node in the tree
    // This is the base case
    if (root == NULL)
    {
        // std::cout << "This node is a root node" << std::endl;
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

        // std::cout << "Finding appropriate leaf node" << std::endl;

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

                // std::cout << "Inserted node, now checking for violation" << std::endl;
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
    // std::cout << "Deleting node: " << deleteKey << std::endl;

    Node *parentNode = root;
    Node *childNode = NULL;

    startWritingNode(parentNode);

    // Start the search for the node to delete
    // std::cout << "Starting search for node" << std::endl;
    while (true)
    {
        if (deleteKey == parentNode->getKey())
        {
            // This is the case when the node to delete is the root
            // std::cout << "Found the node to delete" << std::endl;
            childNode = parentNode;
            break;
        }
        else if (deleteKey < parentNode->getKey())
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
            // std::cout << "Could not find the node" << std::endl;
            return false;
        }
        else if (childNode->getKey() == deleteKey)
        {
            // We've found the node to delete, so lock it and get ready for deletion
            // std::cout << "Found the node to delete" << std::endl;
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
        // std::cout << "Deleted node has no left child" << std::endl;
        // If the node to delete has no left child, we don't have to do much work
        xNode = childNode->getRight();
        startWritingNode(xNode);

        // std::cout << "Transplanting" << std::endl;
        // Transplant the child with the child's right node
        transplant(childNode, childNode->getRight());
    }
    else if (childNode->getRight() == NULL)
    {
        // std::cout << "Deleted node has no right child" << std::endl;

        // If the node to delete has no right child, just replace the left child with the node to delete
        xNode = childNode->getLeft();
        startWritingNode(xNode);

        // std::cout << "Transplanting" << std::endl;
        // Transplant the child with the child's left node
        transplant(childNode, childNode->getLeft());
    }
    else
    {
        // std::cout << "Deleted node has both children" << std::endl;

        // The node to delete has both a left child and a right child
        // Find the minimum value in the node to delete's right tree
        // std::cout << "Finding minimum" << std::endl;
        yNode = minimum(childNode->getRight());
        startWritingNode(yNode);

        // Change the original color to yNode
        // std::cout << "Getting the original color" << std::endl;
        originalColor = yNode->getNodeColor();

        // Get the right children of the yNode
        // std::cout << "Getting the right child of minimum" << std::endl;
        xNode = yNode->getRight();
        startWritingNode(xNode);

        if (yNode->getParent() == childNode)
        {
            // If the minimum node's parent is the node to delete, this makes
            // the transfer process easy
            // std::cout << "Setting xNode's parent to yNode" << std::endl;
            if (xNode != NULL)
            {
                xNode->setParent(yNode);
            }
        }
        else
        {
            // Transplant the right child of the minimum value with y
            transplant(yNode, yNode->getRight());
            yNode->setRight(childNode->getRight());

            // Set the parent of yNode's right child to yNode
            startWritingNode(yNode->getRight());
            yNode->getRight()->setParent(yNode);
            stopWritingNode(yNode->getRight());
        }

        // Transplant the childNode and the yNode
        transplant(childNode, yNode);
        // Set yNode's left children to childNode's left children
        yNode->setLeft(childNode->getLeft());

        // Update yNode's left children to point to yNode
        startWritingNode(yNode->getLeft());
        yNode->getLeft()->setParent(yNode);
        stopWritingNode(yNode->getLeft());

        // Set yNode's color to the deleted node's original color
        yNode->setNodeColor(childNode->getNodeColor());

        stopWritingNode(yNode);
    }

    // std::cout << "Deleting node" << std::endl;

    // Delete childNode
    stopWritingNode(xNode);
    stopWritingNode(childNode);
    delete(childNode);

    // We only need to fix violations if the original color of the transplanted node was black
    if (originalColor == Color::BLACK)
    {
        // std::cout << "Fixing deletion violation" << std::endl;
        fixDeletionViolation(xNode);
    }

    // std::cout << "End delete" << std::endl;
    return true;
}

void RBTree::fixDeletionViolation(Node *deletionNode)
{
    Node *sNode;
    Node *violationNode = deletionNode;

    startWritingNode(violationNode);

    // As long as the violation node is not the root AND the color of the violation node is black
    while (violationNode != root && violationNode->getNodeColor() == Color::BLACK)
    {
        Node *parent = violationNode->getParent();
        startWritingNode(parent);

        // Case for when the violation node is the parent's left child
        if (violationNode == parent->getLeft())
        {
            // Start balancing for the right side of the parent
            sNode = parent->getRight();
            startWritingNode(sNode);

            if (sNode->getNodeColor() == Color::RED)
            {
                // Change the color right child of parent to black and set the parent to red
                sNode->setNodeColor(Color::BLACK);
                parent->setNodeColor(Color::RED);

                // Do a left rotate
                stopWritingNode(sNode);
                stopWritingNode(parent);
                rotateLeft(parent);

                // Update the parent variable
                startWritingNode(sNode);
                parent = sNode->getParent();
                startWritingNode(parent);

                // Update the sNode variable
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
                // Update the color of sNode to be red since both sLeftNode and sRightNode are black
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
                // Case that only the right child of sNode is black
                if(sRightNode->getNodeColor() == Color::BLACK)
                {
                    // Update sLeftNode and sNode colors
                    sLeftNode->setNodeColor(Color::BLACK);
                    sNode->setNodeColor(Color::RED);

                    // Do a right rotate
                    stopWritingNode(sLeftNode);
                    stopWritingNode(sRightNode);
                    stopWritingNode(sNode);

                    rotateRight(sNode);

                    // Update the variables after a rotate
                    sNode = parent->getRight();

                    startWritingNode(sNode);
                    sLeftNode = sNode->getLeft();
                    sRightNode = sNode->getRight();
                    startWritingNode(sLeftNode);
                    startWritingNode(sRightNode);
                }

                stopWritingNode(sLeftNode);
                stopWritingNode(sRightNode);

                // Set sNode's color to be the color of it's parent
                sNode->setNodeColor(parent->getNodeColor());
                // Set the parent's color to black
                parent->setNodeColor(Color::BLACK);

                // Update the color of the right child of sNode
                sRightNode = sNode->getRight();
                startWritingNode(sRightNode);
                sRightNode->setNodeColor(Color::BLACK);
                stopWritingNode(sRightNode);

                // Do a left rotate
                stopWritingNode(sNode);
                stopWritingNode(violationNode);
                stopWritingNode(parent);

                rotateLeft(parent);

                // Set the violationNode to the root
                violationNode = root;
                startWritingNode(violationNode);
            }
        }
        else
        {
            // The case for when the violation node is the parent's right child
            // Execution of correcting the violation is similar to above, but only
            // reversed
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

    // Set the violation node's color to black
    violationNode->setNodeColor(Color::BLACK);
    stopWritingNode(violationNode);
}

void RBTree::fixInsertionViolation(Node *insertionNode)
{
    // std::cout << "Starting check for insertion violation" << std::endl;

    Node *violationNode = insertionNode;
    startWritingNode(violationNode);

    Node *parent = NULL;
    Node *grandParent = NULL;

    parent = violationNode->getParent();  
    startWritingNode(parent);

    // These are the conditions for a violation, so don't stop until we've met all the conditions
    while ((violationNode != root) && (violationNode->getNodeColor() != Color::BLACK) && (parent->getNodeColor() == Color::RED))
    {
        // std::cout << "There is still a violation" << std::endl;

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
                    // std::cout << "Case 1A" << std::endl;

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
                    // std::cout << "Case 1B" << std::endl;

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
                    // std::cout << "Case 1C" << std::endl;

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
                    // std::cout << "Case 2A" << std::endl;

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
                    // std::cout << "Case 2B" << std::endl;

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
                    // std::cout << "Case 2C" << std::endl;

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
    stopWritingNode(root);

    // std::cout << "Finished checking for violation" << std::endl;
}

void RBTree::rotateLeft(Node *rotateNode)
{
    // std::cout << "Starting Left Rotate" << std::endl;

    startWritingNode(rotateNode);
    Node *rotateNodeRight = rotateNode->getRight();

    startWritingNode(rotateNodeRight);

    rotateNode->setRight(rotateNodeRight->getLeft());

    if (rotateNode->getRight() != NULL)
    {
        startWritingNode(rotateNode->getRight());
        rotateNode->getRight()->setParent(rotateNode);
        stopWritingNode(rotateNode->getLeft());
    }

    rotateNodeRight->setParent(rotateNode->getParent());

    if (rotateNode->getParent() == NULL)
    {
        root = rotateNodeRight;
    }
    else
    {
        Node *rotateNodeParent = rotateNode->getParent();
        startWritingNode(rotateNodeParent);

        if (rotateNode == rotateNodeParent->getLeft())
        {
            rotateNodeParent->setLeft(rotateNodeRight);
        }
        else
        {
            rotateNodeParent->setRight(rotateNodeRight);
        }

        stopWritingNode(rotateNodeParent);
    }

    rotateNodeRight->setLeft(rotateNode);
    rotateNode->setParent(rotateNodeRight);

    stopWritingNode(rotateNodeRight);
    stopWritingNode(rotateNode);

    // std::cout << "Ending left rotate" << std::endl;
}

void RBTree::rotateRight(Node *rotateNode)
{
    // std::cout << "Starting right rotate" << std::endl;

    startWritingNode(rotateNode);

    Node *rotateNodeLeft = rotateNode->getLeft();
    startWritingNode(rotateNodeLeft);

    rotateNode->setLeft(rotateNodeLeft->getRight());

    if (rotateNode->getLeft() != NULL)
    {
        startWritingNode(rotateNode->getLeft());
        rotateNode->getLeft()->setParent(rotateNode);
        stopWritingNode(rotateNode->getLeft());
    }

    rotateNodeLeft->setParent(rotateNode->getParent());

    if (rotateNode->getParent() == NULL)
    {
        root = rotateNodeLeft;
    }
    else
    {
        Node *rotateNodeParent = rotateNode->getParent();
        startWritingNode(rotateNodeParent);

        if (rotateNode == rotateNodeParent->getLeft())
        {
            rotateNodeParent->setLeft(rotateNodeLeft);
        }
        else
        {
            rotateNodeParent->setRight(rotateNodeLeft);
        }
        
        stopWritingNode(rotateNodeParent);
    }

    rotateNodeLeft->setRight(rotateNode);
    rotateNode->setParent(rotateNodeLeft);

    stopWritingNode(rotateNodeLeft);
    stopWritingNode(rotateNode);

    // std::cout << "Ending right rotate" << std::endl;
}


void RBTree::transplant(Node *removedNode, Node *newNode)
{
    // std::cout << "Starting transplant" << std::endl;
    if (removedNode->getParent() == NULL)
    {
        // std::cout << "new node is the root" << std::endl;
        root = newNode;
    }
    else if (removedNode == removedNode->getParent()->getLeft())
    {
        // std::cout << "Removed node is the left child of its parent" << std::endl;
        removedNode->getParent()->setLeft(newNode);
    }
    else
    {
        // std::cout << "Removed node is the right child of its parent" << std::endl;
        removedNode->getParent()->setRight(newNode);
    }

    // std::cout << "Setting the parent to the new node" << std::endl;
    if (newNode != NULL)
    {
        newNode->setParent(removedNode->getParent());
    }
}

Node *RBTree::minimum(Node *node)
{
    // std::cout << "Starting minimum" << std::endl;
    startReadingNode(node);
    Node *leftNode = NULL;

    while (node->getLeft() != NULL)
    {
        // std::cout << "Node's left child is not null" << std::endl;
        leftNode = node->getLeft();
        startReadingNode(leftNode);
        stopReadingNode(node);
        node->stopReading();
        node = leftNode;
    }

    // std::cout << "Minimum value is: " << node->getKey() << std::endl;
    stopReadingNode(node);
    return node;
}