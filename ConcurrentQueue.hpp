// @author Benjamin Vogel
#pragma once

#include <pthread.h>
#include <queue>
#include <string>

#include <iostream>

/**
 * An implementation of a thread-safe queue needed for holding the results of different search() operations
 * by concurrent threads.
 **/
template <class T>
class ConcurrentQueue
{
    private:
        std::queue<T> q;
        pthread_mutex_t mutex;

    public:
        ConcurrentQueue()
        {
            q = std::queue<T>();
            pthread_mutex_init(&mutex, NULL);
        }

        ~ConcurrentQueue()
        {
            pthread_mutex_destroy(&mutex);
        }

        void push(T message)
        {
            pthread_mutex_lock(&mutex);
            q.push(message);
            pthread_mutex_unlock(&mutex);
        }

        T pop()
        {
            pthread_mutex_lock(&mutex);
            T message = q.front();
            q.pop();
            pthread_mutex_unlock(&mutex);
            return message;
        }

        bool notempty()
        {
            pthread_mutex_lock(&mutex);
            bool notEmpty = !q.empty();
            pthread_mutex_unlock(&mutex);
            return notEmpty;
        }

};