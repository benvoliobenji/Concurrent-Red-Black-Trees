/**
 * @author Benjamin Vogel
 **/
#include "RBTreeRun.hpp"
#include "Node.hpp"
#include "RBTree.hpp"
#include "FileOutput.hpp"
#include "ConcurrentQueue.hpp"

#include <fstream>
#include <iostream>
#include <pthread.h>
#include <time.h>
#include <chrono>

class ThreadArgs
{
    public:
        ConcurrentQueue<Command> *commands;
        RBTree *rbtree;
        ConcurrentQueue<std::string> *q;

        ThreadArgs(ConcurrentQueue<Command> *newCommands, RBTree *newTree, ConcurrentQueue<std::string> *newQ)
        {
            commands = newCommands;
            rbtree = newTree;
            q = newQ;
        }
};

void* threadRun(void *args)
{
    ThreadArgs *arguments = static_cast<ThreadArgs*>(args);

    ConcurrentQueue<Command> *commands = arguments->commands;
    RBTree *rbtree = arguments->rbtree;
    ConcurrentQueue<std::string> *q = arguments->q;

    while (commands->notempty())
    {
        Command command = commands->pop();
        if (command.getCommand() == CommandType::SEARCH)
        {
            bool success = rbtree->search(command.getNode());
            std::string successString = (success) ? "true" : "false";
            std::string message = "search(" + std::to_string(command.getNode());
            message.append(")-> " + successString);
            q->push(message);
        }
        else if (command.getCommand() == CommandType::INSERT)
        {
            rbtree->insert(command.getNode());
        }
        else if (command.getCommand() == CommandType::DELETE)
        {
            rbtree->deleteNode(command.getNode());
        }
        else
        {
            std::cout << "Not a valid command" << std::endl;
        }
    }

    delete(arguments);
    pthread_exit(NULL);
}

void RBTreeRun::runTree(FileOutput output)
{
    auto start = std::chrono::high_resolution_clock::now();

    // Parser parser = Parser();
    // FileOutput output = parser.parse(argv[1]);
    RBTree fileTree = RBTree();
    ConcurrentQueue<std::string> resultsQueue = ConcurrentQueue<std::string>();

    std::vector<std::shared_ptr<Node>> nodes = output.getNodes();

    for (auto i = nodes.begin(); i != nodes.end(); ++i)
    {
        std::cout << "Node: " << (*i)->getKey() << std::endl;
        fileTree.insertNode(*i);
    }

    ConcurrentQueue<Command> searchCommands = output.getSearchCommands();
    ConcurrentQueue<Command> modifyCommands = output.getModifyCommands();

    int numThreads = output.getNumReadThreads() + output.getnumWriteThreads();
    pthread_t threads[numThreads];
    
    int threadNum = 0;
    for (int i = 0; i < output.getNumReadThreads(); ++i)
    {
        ThreadArgs *arguments = new ThreadArgs(&searchCommands, &fileTree, &resultsQueue);

        int rc = pthread_create(&threads[threadNum], NULL, threadRun, (void *)arguments);
        if (rc)
        {
            std::cout << "ERROR; return code from pthread_create() is " << rc << std::endl;
            delete(arguments);
            exit(-1);
        }
        threadNum++;
    }

    for (int i = 0; i < output.getnumWriteThreads(); ++i)
    {
        ThreadArgs *arguments = new ThreadArgs(&modifyCommands, &fileTree, &resultsQueue);

        int rc = pthread_create(&threads[threadNum], NULL, threadRun, (void *)arguments);
        if (rc)
        {
            std::cout << "ERROR; return code from pthread_create() is " << rc << std::endl;
            delete(arguments);
            exit(-1);
        }
        threadNum++;
    }

    for(int t = 0; t < numThreads; t++)
    {
        pthread_join(threads[t], NULL);
    }

    auto done = std::chrono::high_resolution_clock::now();

    std::cout << "Time Elapsed: " << std::chrono::duration_cast<std::chrono::milliseconds>(done - start).count() << std::endl;

    while (resultsQueue.notempty())
    {
        std::string message = resultsQueue.pop();
        std::cout << message << std::endl;
    }

    fileTree.print();
}