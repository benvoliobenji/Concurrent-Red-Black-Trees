#pragma once

#include <fstream>
#include "FileOutput.hpp"
#include "Node.hpp"

class Parser
{
    private:
        FileOutput output;

        void parseNodes();

        void parseThreads();

        void parseCommands();

    public:
        Parser()
        { 
            output = FileOutput();
        }

        FileOutput parse(std::ifstream fileToParse);
};