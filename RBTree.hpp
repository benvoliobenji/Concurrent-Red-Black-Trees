#pragma once

#include "Node.hpp"

class RBTree
{
    private:
        Node *root;

        void startReadingNode(Node *readingNode)
        {
            if (readingNode != NULL)
            {
                readingNode->startReading();
            }
        }

        void stopReadingNode(Node *readingNode)
        {
            if (readingNode != NULL)
            {
                readingNode->stopReading();
            }
        }

        void startWritingNode(Node *writingNode)
        {
            if (writingNode != NULL)
            {
                writingNode->startWriting();
            }
        }

        void stopWritingNode(Node *writingNode)
        {
            if (writingNode != NULL)
            {
                writingNode->stopWriting();
            }
        }

        void fixInsertionViolation(Node *insertionNode);

        void fixDeletionViolation(Node *deletionNode);

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
