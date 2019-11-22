#pragma once

#include "FileOutput.hpp"
#include "Node.hpp"

class Parser
{
    private:
        FileOutput output;

        void parseNodes(std::string nodeString);

        void parseThreads(std::vector<std::string> threadsToParse);

        void parseCommands(std::string commandsToParse);

    public:
        Parser()
        { 
            output = FileOutput();
        }

        FileOutput parse(const char *fileToParse);
};