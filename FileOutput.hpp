// @author Benjamin Vogel
#pragma once

#include <vector>
#include <string>
#include <memory>

#include "Node.hpp"
#include "Command.hpp"

/**
 * A data structure designed to hold all the necessary
 * components for performing operations on a synchronous red-black tree
 **/
class FileOutput
{
    private:
        std::vector<std::shared_ptr<Node>> initialTree;
        std::vector<std::vector<Command>> threadCommands;

    public:
        FileOutput()
        {
            initialTree = std::vector<std::shared_ptr<Node>>();
            threadCommands = std::vector<std::vector<Command>>();
        }

        void addNode(std::shared_ptr<Node> newNode) { initialTree.push_back(newNode); }

        std::vector<std::shared_ptr<Node>> getNodes() { return initialTree; }

        void setNumThreads(int numThreads)
        {
            for (int i = 0; i < numThreads; i++)
            {
                std::vector<Command> commandThread = std::vector<Command>();
                threadCommands.push_back(commandThread);
            }
        }
        
        void addCommand(Command newCommand)
        {
            int thread = newCommand.getThreadNum();

            threadCommands[thread - 1].push_back(newCommand);
        }
        
        std::vector<std::vector<Command>> getCommands() { return threadCommands; }
        
};