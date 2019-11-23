#include "Node.hpp"
#include "RBTree.hpp"
#include "Parser.hpp"
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
        std::vector<Command> commands;
        RBTree rbtree;
        ConcurrentQueue q;

        ThreadArgs(std::vector<Command> newCommands, RBTree &newTree, ConcurrentQueue &newQ)
        {
            commands = newCommands;
            rbtree = newTree;
            q = newQ;
        }
};

void* threadRun(void *args)
{
    ThreadArgs *arguments = static_cast<ThreadArgs*>(args);

    std::vector<Command> commands = arguments->commands;
    RBTree rbtree = arguments->rbtree;
    ConcurrentQueue q = arguments->q;

    for (auto i = commands.begin(); i != commands.end(); ++i)
    {
        Command command = (*i);
        if (command.getCommand() == CommandType::SEARCH)
        {
            bool success = rbtree.search(command.getNode());
            std::string successString = (success) ? "true" : "false";
            std::string message = "thread" + command.getThreadNum();
            message += ", search(" + command.getNode();
            message += ")-> " + successString;
            std::cout << message << std::endl;
            q.push(message);
        }
        else if (command.getCommand() == CommandType::INSERT)
        {
            rbtree.insert(command.getNode());
        }
        else if (command.getCommand() == CommandType::DELETE)
        {
            rbtree.deleteNode(command.getNode());
        }
        else
        {
            std::cout << "Not a valid command" << std::endl;
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    // RBTree tree = RBTree();

    // Node *node = new Node(7, Color::BLACK);
    // tree.insertNode(node);

    // node = new Node(3, Color::BLACK);
    // tree.insertNode(node);

    // node = new Node(18, Color::RED);
    // tree.insertNode(node);

    // node = new Node(10, Color::BLACK);
    // tree.insertNode(node);

    // node = new Node(8, Color::RED);
    // tree.insertNode(node);

    // node = new Node(11, Color::RED);
    // tree.insertNode(node);

    // node = new Node(22, Color::BLACK);
    // tree.insertNode(node);

    // node = new Node(26, Color::RED);
    // tree.insertNode(node);

    // tree.print();

    // bool found = tree.search(7);
    // std::cout << "Finding node 7: " << found << std::endl;

    // tree.deleteNode(7);

    // found = tree.search(7);
    // std::cout << "Finding node 7: " << found << std::endl;

    // tree.print();

    auto start = std::chrono::high_resolution_clock::now();

    Parser parser = Parser();
    FileOutput output = parser.parse(argv[1]);
    RBTree fileTree = RBTree();
    ConcurrentQueue q = ConcurrentQueue();

    std::vector<Node *> nodes = output.getNodes();

    for (auto i = nodes.begin(); i != nodes.end(); ++i)
    {
        std::cout << "Node: " << (*i)->getKey() << std::endl;
        fileTree.insertNode(*i);
    }

    std::vector<std::vector<Command>> commands = output.getCommands();

    pthread_t threads[commands.size()];
    std::cout << "Size of the commands: " << commands.size() << std::endl;
    
    int threadNum = 0;
    for (auto i = commands.begin(); i != commands.end(); ++i)
    {
        std::vector<Command> threadCommand = *i;
        ThreadArgs *arguments = new ThreadArgs(threadCommand, fileTree, q);

        int rc = pthread_create(&threads[threadNum], NULL, threadRun, (void *)arguments);
        if (rc)
        {
            std::cout << "ERROR; return code from pthread_create() is " << rc << std::endl;
            exit(-1);
        }
        threadNum++;
    }

    for(size_t t = 0; t < commands.size(); t++)
    {
        pthread_join(threads[t], NULL);
    }

    auto done = std::chrono::high_resolution_clock::now();

    std::cout << "Time Elapsed: " << std::chrono::duration_cast<std::chrono::milliseconds>(done - start).count() << std::endl;
    std::cout << q.notempty() << std::endl;

    while (q.notempty())
    {
        std::string message = q.pop();
        std::cout << message << std::endl;
    }

    fileTree.print();
}