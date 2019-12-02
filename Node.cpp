// @author Benjamin Vogel
#include "Node.hpp"

    Node::Node (int initKey)
    {
        key = initKey;
        parent = left = right = NULL;
        nodeColor = Color::RED;
        pthread_mutex_init(&mutex, NULL);
        pthread_cond_init(&OKtoRead, NULL);
        pthread_cond_init(&OKtoWrite, NULL);
        currentlyReading = 0;
        currentlyWriting = false;
        waitingReaders = waitingWriters = 0;
    }

    Node::Node (int initKey, Color color)
    {
        key = initKey;
        parent = left = right = NULL;
        nodeColor = color;
        pthread_mutex_init(&mutex, NULL);
        pthread_cond_init(&OKtoRead, NULL);
        pthread_cond_init(&OKtoWrite, NULL);
        currentlyReading = 0;
        currentlyWriting = false;
        waitingReaders = waitingWriters = 0;
    }

    Node::Node(int initKey, std::shared_ptr<Node> parentNode, std::shared_ptr<Node> leftChild, std::shared_ptr<Node> rightChild, Color color)
    {
        key = initKey;
        parent = parentNode;
        left = leftChild;
        right = rightChild;
        nodeColor = color;
        pthread_mutex_init(&mutex, NULL);
        pthread_cond_init(&OKtoRead, NULL);
        pthread_cond_init(&OKtoWrite, NULL);
        currentlyReading = 0;
        currentlyWriting = false;
        waitingReaders = waitingWriters = 0;
    }

    Node::~Node()
    {
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&OKtoRead);
        pthread_cond_destroy(&OKtoWrite);
    }

    void Node::startReading()
    {
        lockNode();
        if (currentlyWriting || waitingWriters > 0)
        {
            waitingReaders++;
            pthread_cond_wait(&OKtoRead, &mutex);
            waitingReaders--;
        }

        currentlyReading++;
        pthread_cond_signal(&OKtoRead);
        unlockNode();
    }

    void Node::stopReading()
    {
        lockNode();
        currentlyReading--;
        if (currentlyReading == 0)
        {
            pthread_cond_signal(&OKtoWrite);
        }
        unlockNode();
    }

    void Node::startWriting()
    {
        lockNode();
        if (currentlyReading > 0 || currentlyWriting)
        {
            waitingWriters++;
            pthread_cond_wait(&OKtoWrite, &mutex);
            waitingWriters--;
        }

        currentlyWriting = true;
        unlockNode();
    }

    void Node::stopWriting()
    {
        lockNode();
        currentlyWriting = false;
        if (waitingReaders > 0)
        {
            pthread_cond_signal(&OKtoRead);
        }
        else
        {
            pthread_cond_signal(&OKtoWrite);
        }
        unlockNode();
    }