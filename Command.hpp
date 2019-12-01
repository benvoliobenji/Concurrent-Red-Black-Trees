// @author Benjamin Vogel
#pragma once

#include <vector>

/**
 * An enum to provide better representation of the different commands that will be run
 **/
enum class CommandType
{
    SEARCH,
    INSERT,
    DELETE
};

/**
 * A data structure used to contain the information needed to perform a successful command
 * in the threads
 **/
class Command
{
    private:
        int threadNum;
        CommandType command;
        int nodeAffected;

    public:
        Command(int newThreadNum, CommandType newCommand, int newNodeAffected)
        {
            threadNum = newThreadNum;
            command = newCommand;
            nodeAffected = newNodeAffected;
        }

        Command() {}

        void setThreadNum(int newThreadNum) { threadNum = newThreadNum; }
        int getThreadNum() { return threadNum; }

        void setCommand(CommandType newCommand) { command = newCommand; }
        CommandType getCommand() { return command; }

        void setNode(int newNode) { nodeAffected = newNode; }
        int getNode() { return nodeAffected; }

};