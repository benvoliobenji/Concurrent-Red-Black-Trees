#pragma once

#include <pthread.h>
#include <queue>
#include <string>

class ConcurrentQueue
{
    private:
        std::queue<std::string> q;
        pthread_mutex_t mutex;

    public:
        ConcurrentQueue()
        {
            q = std::queue<std::string>();
            pthread_mutex_init(&mutex, NULL);
        }

        ~ConcurrentQueue()
        {
            pthread_mutex_destroy(&mutex);
        }

        void push(std::string message)
        {
            pthread_mutex_lock(&mutex);
            q.push(message);
            pthread_mutex_unlock(&mutex);
        }

        std::string pop()
        {
            pthread_mutex_lock(&mutex);
            std::string message = q.front();
            q.pop();
            pthread_mutex_unlock(&mutex);
            return message;
        }

        bool notempty()
        {
            return !q.empty();
        }

};