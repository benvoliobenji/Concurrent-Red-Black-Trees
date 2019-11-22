#pragma once

#include <vector>

enum class CommandType
{
    SEARCH,
    INSERT,
    DELETE
};

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

        void setThreadNum(int newThreadNum) { threadNum = newThreadNum; }
        int getThreadNum() { return threadNum; }

        void setCommand(CommandType newCommand) { command = newCommand; }
        CommandType getCommand() { return command; }

        void setNode(int newNode) { nodeAffected = newNode; }
        int getNode() { return nodeAffected; }

};