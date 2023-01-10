/**
 * @author Benjamin Vogel
 **/
#include "RBTreeRun.hpp"
#include "Node.hpp"
#include "RBTree.hpp"
#include "FileOutput.hpp"
#include "ConcurrentQueue.hpp"

#include <iostream>
#include <pthread.h>
#include <chrono>

/**
 * A structure used to pass arguments to the threads
 **/
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

/**
 * The method the threads run
 **/
void* threadRun(void *args)
{
    // Cast the arguments to the structure and grab the data we need
    ThreadArgs *arguments = static_cast<ThreadArgs*>(args);

    ConcurrentQueue<Command> *commands = arguments->commands;
    RBTree *rbtree = arguments->rbtree;
    ConcurrentQueue<std::string> *q = arguments->q;

    // Now run commands until there are none left to run
    while (commands->notempty())
    {
        Command command = commands->pop();
        if (command.getCommand() == CommandType::SEARCH)
        {
            bool success = rbtree->search(command.getNode());
            std::string successString = (success) ? "true" : "false";
            std::string message = "search(" + std::to_string(command.getNode());
            message.append(")-> " + successString);
            message.append(", performed by thread: " + std::to_string(pthread_self()));
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

    // Free up space by deleting the data structure
    delete(arguments);
    pthread_exit(NULL);
}

void RBTreeRun::runTree(FileOutput output)
{
    // Start timing
    auto start = std::chrono::high_resolution_clock::now();

    // Create our needed data structures
    RBTree fileTree = RBTree();
    ConcurrentQueue<std::string> resultsQueue = ConcurrentQueue<std::string>();

    // Build the initial tree
    std::vector<std::shared_ptr<Node>> nodes = output.getNodes();

    for (auto i = nodes.begin(); i != nodes.end(); ++i)
    {
        fileTree.insertNode(*i);
    }

    fileTree.print();

    ConcurrentQueue<Command> searchCommands = output.getSearchCommands();
    ConcurrentQueue<Command> modifyCommands = output.getModifyCommands();

    int numThreads = output.getNumReadThreads() + output.getnumWriteThreads();
    pthread_t threads[numThreads];
    
    // Initialize the search threads
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

    // Initialize the modify threads
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

    // Wait for them to finish
    for(int t = 0; t < numThreads; t++)
    {
        pthread_join(threads[t], NULL);
    }

    auto done = std::chrono::high_resolution_clock::now();

    std::cout << "Time Elapsed: " << std::chrono::duration_cast<std::chrono::milliseconds>(done - start).count() << std::endl;

    // Print out the results of the searches
    while (resultsQueue.notempty())
    {
        std::string message = resultsQueue.pop();
        std::cout << message << std::endl;
    }

    // Print out the finished tree
    fileTree.print();
}