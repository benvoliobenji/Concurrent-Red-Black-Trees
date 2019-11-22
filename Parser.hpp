#pragma once

#include <fstream>
#include "FileOutput.hpp"

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