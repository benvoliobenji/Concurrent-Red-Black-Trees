// @author Benjamin Vogel
#pragma once

#include <vector>
#include <string>
#include <memory>

#include "Node.hpp"
#include "Command.hpp"
#include "ConcurrentQueue.hpp"

/**
 * A data structure designed to hold all the necessary
 * components for performing operations on a synchronous red-black tree
 **/
class FileOutput
{
    private:
        std::vector<std::shared_ptr<Node>> initialTree;
        int numReadThreads = 0;
        int numWriteThreads = 0;
        ConcurrentQueue<Command> readQueue;
        ConcurrentQueue<Command> modifyQueue;

    public:
        FileOutput()
        {
            initialTree = std::vector<std::shared_ptr<Node>>();
            readQueue = ConcurrentQueue<Command>();
            modifyQueue = ConcurrentQueue<Command>();
            numReadThreads = 0;
            numWriteThreads = 0;
        }

        void addNode(std::shared_ptr<Node> newNode) { initialTree.push_back(newNode); }

        std::vector<std::shared_ptr<Node>> getNodes() { return initialTree; }

        void setNumReadThreads(int numThreads) { numReadThreads = numThreads; }
        int getNumReadThreads() { return numReadThreads; }

        void setNumWriteThreads(int numThreads) { numWriteThreads = numThreads; }
        int getnumWriteThreads() {return numWriteThreads; }
        
        void addSearchCommand(Command newCommand)
        {
            readQueue.push(newCommand);
        }

        void addModifyCommand(Command newCommand)
        {
            modifyQueue.push(newCommand);
        }

        ConcurrentQueue<Command> getSearchCommands() { return readQueue; }
        ConcurrentQueue<Command> getModifyCommands() { return modifyQueue; }    
};