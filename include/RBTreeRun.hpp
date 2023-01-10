/**
 * @author Benjamin Vogel
 **/
#pragma once

#include "FileOutput.hpp"

/**
 * A static class that holds a static method to run the tree based on the parsed data
 **/
class RBTreeRun
{
    public:
        /**
         * Builds the initial tree, spawns the worker threads,
         * and sets them off to work while waiting for them all to wrap up.
         * It then reports the elapsed time for the whole processing and exits
         * 
         * @param output: A FileOutput object that contains all the needed information to build
         * and run a concurrent red black tree.
         **/
        static void runTree(FileOutput output);
};