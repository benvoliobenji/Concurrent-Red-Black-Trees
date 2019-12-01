#pragma once

#include <pthread.h>
#include <queue>
#include <string>

#include <iostream>

class ConcurrentQueue
{
    private:
        std::queue<std::string>* q;
        pthread_mutex_t mutex;

    public:
        ConcurrentQueue()
        {
            q = new std::queue<std::string>();
            pthread_mutex_init(&mutex, NULL);
        }

        ~ConcurrentQueue()
        {
            pthread_mutex_destroy(&mutex);
        }

        void push(std::string message)
        {
            pthread_mutex_lock(&mutex);
            q->push(message);
            pthread_mutex_unlock(&mutex);
        }

        std::string pop()
        {
            pthread_mutex_lock(&mutex);
            std::string message = q->front();
            q->pop();
            pthread_mutex_unlock(&mutex);
            return message;
        }

        bool notempty()
        {
            pthread_mutex_lock(&mutex);
            bool notEmpty = !q->empty();
            pthread_mutex_unlock(&mutex);
            return notEmpty;
        }

};