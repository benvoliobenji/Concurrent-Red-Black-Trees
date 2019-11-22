#pragma once

#include <vector>
#include <string>

#include "Node.hpp"
#include "Command.hpp"

class FileOutput
{
    private:
        std::vector<Node *> initialTree;
        std::vector<std::vector<Command>> threadCommands;

    public:
        FileOutput()
        {
            initialTree = std::vector<Node *>();
            threadCommands = std::vector<std::vector<Command>>();
        }

        void addNode(Node *newNode) { initialTree.push_back(newNode); }

        std::vector<Node *> getNodes() { return initialTree; }

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