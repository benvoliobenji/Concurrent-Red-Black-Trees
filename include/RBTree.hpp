/**
 * @author Benjamin Vogel
 **/
#pragma once

#include "Node.hpp"

/**
 * The class that contains the functions needed to operate and display critical functions
 * needed for a red-black tree.
 **/
class RBTree
{
    private:
        std::shared_ptr<Node> root;

        /**
         * A helper function used to work in NULL checking, allows for added thread safety
         * @param readingNode: The node to start reading.
         **/
        void startReadingNode(std::shared_ptr<Node> readingNode)
        {
            if (readingNode != NULL)
            {
                readingNode->startReading();
            }
        }

        /**
         * A helper function used to work in NULL checking, allows for added thread safety
         * @param readingNode: The node to stop reading.
         **/
        void stopReadingNode(std::shared_ptr<Node> readingNode)
        {
            if (readingNode != NULL)
            {
                readingNode->stopReading();
            }
        }

        /**
         * A helper function used to work in NULL checking, allows for added thread safety
         * @param writingNode: The node to start writing.
         **/
        void startWritingNode(std::shared_ptr<Node> writingNode)
        {
            if (writingNode != NULL)
            {
                writingNode->startWriting();
            }
        }

        /**
         * A helper function used to work in NULL checking, allows for added thread safety
         * @param writingNode: The node to start writing.
         **/
        void stopWritingNode(std::shared_ptr<Node> writingNode)
        {
            if (writingNode != NULL)
            {
                writingNode->stopWriting();
            }
        }

        /**
         * A recursive function used for printing out the red-black tree to the user.
         * @param printNode: The center node to print, the right child and the left child will also call printREC
         * @param level: The current level of the node in the red-black tree
         **/
        void printREC(std::shared_ptr<Node> printNode, int level);

        /**
         * A helper function for insert(), this function checks if the inserted node causes any violations in the red-black
         * tree and attempts to fix those violations within the tree. There are several cases for a violation, and there is documentation
         * both online and in the code to highlight the solutions for those different cases.
         * @param insertionNode: The inserted node that may cause a violation.
         **/
        void fixInsertionViolation(std::shared_ptr<Node> insertionNode);

        /**
         * A helper function for delete(), this function checks if deleting a specific node caused any violations in the red-black tree
         * and will attempt to fix thos violations within the tree. Similar to insert(), there are several cases in fixing a deletion violation,
         * and there is documentation both online and in the code that address the solutions for those different cases.
         * @param deletionNode: The deleted node that may now cause a violation.
         **/
        void fixDeletionViolation(std::shared_ptr<Node> deletionNode);

        /**
         * Another helper function used in both fixInsertionViolation() and fixDeletionViolation(). This function rotates the tree left
         * based on the parameter node.
         * @param rotateNode: The node to rotate left around
         **/
        void rotateLeft(std::shared_ptr<Node> rotateNode);

        /**
         * Another helper function used in both fixInsertionViolation() and fixDeletionViolation(). This function rotates the tree right
         * based on the parameter node.
         * @param rotateNode: The node to rotate right around
         **/
        void rotateRight(std::shared_ptr<Node> rotateNode);

        /**
         * A helper function to reduce code clutter. This moves the child pointers from the removedNode to the newNode
         * and adjusts other pointers accordingly
         * @param removedNode: The node that will be replaced
         * @param newNode: The new node that will be in the place of removedNode
         **/
        void transplant(std::shared_ptr<Node> removedNode, std::shared_ptr<Node> newNode);

        /**
         * A helper function that finds the minimum node closest to the parameter node's key
         * @param node: The node to find the minimum of
         * @returns A pointer to the minimum value node
         **/
        std::shared_ptr<Node> minimum(std::shared_ptr<Node> node);

    public:
        RBTree() { root = NULL; };

        // ~RBTree() { delete(root); }

        /**
         * Prints out the red-black tree in full. Empty children are represented by "f"
         **/
        void print();
        
        /**
         * Searches for a node with the provided key.
         * @param searchKey: The key of the node to look for
         * @returns If the node is in the current red-black tree
         **/
        bool search(int searchKey);

        /**
         * Inserts a new node with a provided key.
         * @param insertKey: The key of a new node to insert
         * @returns The success of inserting the new node
         **/
        bool insert(int insertKey);

        /**
         * A variation of insert(), only an entire node is provided
         * @param insertNode: The node pointer to insert
         * @returns The success of inserting the new node
         **/
        bool insertNode(std::shared_ptr<Node> insertNode);

        /**
         * Deletes the provided node from the tree.
         * @param deleteKey: The key of the node to delete
         * @returns The success of deleting the desired node
         **/
        bool deleteNode(int deleteKey);
};
