// @author Benjamin Vogel
#pragma once

#include <pthread.h>
#include <queue>

#include <iostream>
/**
 * An implementation of a thread-safe queue needed for holding either Command objects
 * for the threads to process or the results of search() queries.
 * Made generic for that exact reason.
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