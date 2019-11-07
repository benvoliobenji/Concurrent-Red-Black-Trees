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
        bool currentlyReading;
        bool currentlyWriting;
        pthread_mutex_t mutex;
        pthread_cond_t OKtoRead;

    public:

    Node (int initKey)
    {
        key = initKey;
        parent = left = right = NULL;
        nodeColor = Color::RED;
        pthread_mutex_init(&mutex, NULL);
        pthread_cond_init(&OKtoRead, NULL);
        currentlyReading = currentlyWriting = false;
    }

    Node (int initKey, Color color)
    {
        key = initKey;
        parent = left = right = NULL;
        nodeColor = color;
        pthread_mutex_init(&mutex, NULL);
        pthread_cond_init(&OKtoRead, NULL);
        currentlyReading = currentlyWriting = false;
    }

    Node(int initKey, Node *parentNode, Node *leftChild, Node *rightChild, Color color)
    {
        key = initKey;
        parent = parentNode;
        left = leftChild;
        right = rightChild;
        nodeColor = color;
        pthread_mutex_init(&mutex, NULL);
        pthread_cond_init(&OKtoRead, NULL);
        currentlyReading = currentlyWriting = false;
    }

    void lockNode() { pthread_mutex_lock(&mutex); }
    void unlockNode() { pthread_mutex_unlock(&mutex); }

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