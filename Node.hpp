// @author Benjamin Vogel
#pragma once

#include <memory>
#include <pthread.h>

/**
 * The enum to represent the color of the nodes
 **/ 
enum class Color
{
    BLACK,
    RED
};

/**
 * The object that represents the node. Each node is thread-safe and emulates a readers-writers solution with equal priority
 * Each node object hold it's key, it's color, a pointer to it's parent node, a pointer to it's left and right children, and appropriate
 * variables and conditions needed to enforce thread safety.
 **/
class Node
{
    private:
        int key;
        std::shared_ptr<Node> parent;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        Color nodeColor;
        int currentlyReading = 0;
        bool currentlyWriting;
        int waitingWriters = 0;
        int waitingReaders = 0;
        pthread_mutex_t mutex;
        pthread_cond_t OKtoRead;
        pthread_cond_t OKtoWrite;

        /**
         * A helper function used within the class to lock the mutex
         **/
        void lockNode() { pthread_mutex_lock(&mutex); }

        /**
         * A helper function used within the class to unlock the mutex
         **/
        void unlockNode() { pthread_mutex_unlock(&mutex); }

    public:
        /**
         * Initializer that only takes the key value. Color is initially set to red
         * @param initKey: The key value of the node to construct
         **/
        Node (int initKey);

        /**
         * Initializer that takes both the key value and the initial color of the node
         * @param initKey: The key value of the node to construct
         * @param color: The intiiali color of the node. There is no garuntee the color will stay that way after insertion
         **/
        Node (int initKey, Color color);

        /**
         * Iniitializer that takes all parameters. Never used but useful in the case that it might
         * @param initKey: The key value of the node to construct
         * @param parentNode: The constructed node's new parent
         * @param leftChild: The left child of the new node
         * @param rightChild: The right child of the new node
         * @param color: The initial color of the new node.
         **/
        Node(int initKey, std::shared_ptr<Node> parentNode, std::shared_ptr<Node> leftChild, std::shared_ptr<Node> rightChild, Color color);

        ~Node();

        /**
         * The function used for threads that are attempting to read the node only.
         * Threads will be blocked on this function if a writer is currently writing or
         * if there is a waiting writer.
         * If it is unblocked and can read, it will signal other readers that it is okay to read.
         **/
        void startReading();
        /**
         * The function used for threads that are done reading the node.
         * If there are no other readers currently reading, then it signals any waiting writer nodes to start writing.
         **/
        void stopReading();

        /**
         * The function used for threads that are attempting to modify the node (or write).
         * Threads will be blocked on this function if there is a writer currently writing or there are readers currently reading.
         **/
        void startWriting();
        /**
         * The function used for threads that are done writing to the node.
         * If there are readers waiting, then it signals the readers to start reading, else it signals any waiting writers.
         **/
        void stopWriting();

        int getKey() { return key; }
        void setKey(int newKey) { key = newKey; }

        std::shared_ptr<Node> getParent() { return parent; }
        void setParent(std::shared_ptr<Node> newParent) { parent = newParent; }

        std::shared_ptr<Node> getLeft() { return left; }
        void setLeft(std::shared_ptr<Node> newLeft) { left = newLeft; }


        std::shared_ptr<Node> getRight() { return right; }
        void setRight(std::shared_ptr<Node> newRight) { right = newRight; }


        Color getNodeColor() { return nodeColor; }
        void setNodeColor(Color newNodeColor) { nodeColor = newNodeColor; }

        inline bool operator==(const Node &compareNode) {return key == compareNode.key;}
};