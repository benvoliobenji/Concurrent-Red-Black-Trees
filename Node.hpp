#pragma once

#include <pthread.h>

enum class Color
{
    BLACK,
    RED
};

class Node
{
    private:
        int key;
        Node *parent;
        Node *left;
        Node *right;
        Color nodeColor;
        int currentlyReading = 0;
        bool currentlyWriting;
        int waitingWriters = 0;
        int waitingReaders = 0;
        pthread_mutex_t mutex;
        pthread_cond_t OKtoRead;
        pthread_cond_t OKtoWrite;

        void lockNode() { pthread_mutex_lock(&mutex); }
        void unlockNode() { pthread_mutex_unlock(&mutex); }

    public:

    Node (int initKey);

    Node (int initKey, Color color);

    Node(int initKey, Node *parentNode, Node *leftChild, Node *rightChild, Color color);

    ~Node();

    void startReading();
    void stopReading();

    void startWriting();
    void stopWriting();

    int getKey() { return key; }
    void setKey(int newKey) { key = newKey; }

    Node *getParent() { return parent; }
    void setParent(Node *newParent) { parent = newParent; }

    Node *getLeft() { return left; }
    void setLeft(Node *newLeft) { left = newLeft; }


    Node *getRight() { return right; }
    void setRight(Node *newRight) { right = newRight; }


    Color getNodeColor() { return nodeColor; }
    void setNodeColor(Color newNodeColor) { nodeColor = newNodeColor; }
};