// @author Benjamin Vogel
#pragma once

#include "FileOutput.hpp"
#include "Node.hpp"

/**
 * A basic parser class to parse the input on a provided file
 **/
class Parser
{
    private:
        FileOutput output;

        /**
         * Parses the nodes of a red-black tree from a given string
         * @param nodeString: The line in the file that contains the nodes in a red-black tree
         **/
        void parseNodes(std::string nodeString);

        /**
         * Parses the commands the threads will operate on the tree
         * @param commandsToParse: The line in the file that contains the commands to operate on a red-black tree
         **/
        void parseCommands(std::string commandsToParse);

    public:
        Parser()
        { 
            output = FileOutput();
        }

        /**
         * Parses the necessary nodes, threads, and commands needed to perform a red-black tree synchronous operation.
         * @param fileToParse: The name of the file to open and read from
         * @returns An object containing all the necessary information for running a synchronous red-black tree
         **/
        void parse(const char *fileToParse);
};