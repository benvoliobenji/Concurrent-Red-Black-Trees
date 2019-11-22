#include <sstream>
#include <iostream>

#include "Parser.hpp"
#include "trim.h"

FileOutput Parser::parse(std::ifstream fileToParse)
{
    std::string line;
    if (!fileToParse.is_open())
    {
        perror("Error opening file");
        return output;
    }

    std::vector<std::string> nodes;
    std::vector<std::string> threads;
    std::vector<std::string> commands;

    bool nodesRead = false;
    bool threadsRead = false;
    bool commandsRead = false;

    while (std::getline(fileToParse, line))
    {
        if (line.compare("\n") == 0)
        {
            nodesRead = true;

            threadsRead = (nodesRead && (threads.size() > 0)) ? true : false;

            if (threadsRead)
            {
                parseThreads(threads);
            }
        }
        else
        {
            if (!nodesRead)
            {
                // Since all the nodes are on one line with no return, we can just parse this line and it'll be correct
                parseNodes(line);
                nodesRead = true;
            }
            else
            {
                if (!threadsRead)
                {
                    std::string token;

                    token = trim(line);
                    threads.push_back(token);
                }
                else
                {
                    if (!commandsRead)
                    {
                        // Same as parseNodes, all the commands are on one line, so we don't have to worry about it
                        parseCommands(line);
                        commandsRead = true;
                    }
                    else
                    {
                        std::cout << "More data in this file: " << line << std::endl;
                    }   
                } 
            }
        }
    }
    return output;
}

void Parser::parseNodes(std::string nodeString)
{
    std::stringstream ss(nodeString);

    std::string token;

    while (std::getline(ss, token, ','))
    {
        // We don't terribly care if this is a null node, the balancing will do it for us
        if (token.compare("f") != 0)
        {
            token = trim(token);
            char color = token[token.size() - 1];

            Color nodeColor = (color == 'r') ? Color::RED : Color::BLACK;

            int value = std::stoi(token.substr(0, token.size() - 1));

            Node *newNode = new Node(value, nodeColor);

            output.addNode(newNode);
        }
    }
}

void Parser::parseThreads(std::vector<std::string> threadsToParse)
{
    output.setNumThreads(threadsToParse.size());
}

void Parser::parseCommands(std::string commandsToParse)
{
    std::stringstream ss(commandsToParse);

    std::string lineToken;

    while (std::getline(ss, lineToken, '|'))
    {
        if (lineToken.size() > 0)
        {
            lineToken = trim(lineToken);
            std::stringstream ss2(lineToken);

            std::string commandToken;

            Command newCommand = Command();
            bool threadRead = false;

            while(std::getline(ss2, commandToken, ','))
            {
                commandToken = trim(commandToken);

                if (!threadRead)
                {
                    int threadNum = (int)commandToken[commandToken.size() - 1];
                    newCommand.setThreadNum(threadNum);
                }
                else
                {
                    std::string command = commandToken.substr(0, 6);

                    if (command.compare("search") == 0)
                    {
                        newCommand.setCommand(CommandType::SEARCH);
                    }
                    else if (command.compare("insert") == 0)
                    {
                        newCommand.setCommand(CommandType::INSERT);
                    }
                    else if (command.compare("delete") == 0)
                    {
                        newCommand.setCommand(CommandType::DELETE);
                    }
                    else
                    {
                        std::cout << "Error, not a valid command: " << command << std::endl;
                    }

                    std::string nodeAffected = commandToken.substr(5, commandToken.size() - 6);
                    int node = std::stoi(nodeAffected.substr(1, nodeAffected.size() - 3));
                    newCommand.setNode(node);
                }
            }

            output.addCommand(newCommand);
        }
    }
}