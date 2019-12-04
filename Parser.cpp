/**
 * @author Benjamin Vogel
 **/
#include <bits/stdc++.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <memory>

#include "Parser.hpp"
#include "trim.h"
#include "RBTreeRun.hpp"

void Parser::parse(const char *fileToParse)
{
    std::fstream file;
    file.open(fileToParse);

    std::string line;
    if (!file.is_open())
    {
        perror("Error opening file");
        return;
    }

    std::vector<std::string> nodes;
    std::vector<std::string> threads;
    std::vector<std::string> commands;

    bool nodesRead = false;
    bool threadsRead = false;
    bool commandsRead = false;

    RBTreeRun tree = RBTreeRun();

    while (std::getline(file, line))
    {
        // TODO: Check for lines between different tests
        std::cout << "Got Line: " << line << std::endl;
        line = trim(line);

        if (line.empty())
        {
            std::cout << "Empty Line" << std::endl;

            threadsRead = (nodesRead && (threads.size() > 0)) ? true : false;

            if (threadsRead)
            {
                std::cout << "Parsing threads" << std::endl;
                parseThreads(threads);
            }
        }
        else
        {
            if (!nodesRead)
            {
                std::string testLine = line.substr(0, 4);
                std::transform(testLine.begin(), testLine.end(), testLine.begin(), ::tolower);

                std::cout << testLine << std::endl;

                // Check to make sure that the first line we read in is NOT the test case header line
                if (testLine.compare("test") != 0)
                {
                    std::cout << "Parsing Nodes" << std::endl;
                    // Since all the nodes are on one line with no return, we can just parse this line and it'll be correct
                    parseNodes(line);
                    nodesRead = true;
                }
            }
            else
            {
                if (!threadsRead)
                {
                    std::string token;

                    token = trim(line);
                    threads.push_back(token);

                    std::cout << "Added thread: " << token << std::endl;
                }
                else
                {
                    if (!commandsRead)
                    {
                        std::cout << "Parsing Commands" << std::endl;
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

        if (nodesRead && threadsRead && commandsRead)
        {
            // This is a new tree to run, so run it
            tree.runTree(output);
            output = FileOutput();
            nodesRead = false;
            threadsRead = false;
            commandsRead = false;
        }
    }
    return;
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

            // Check to make sure it's a valid node
            if (isalpha(token[token.size() - 1]))
            {
                char color = token[token.size() - 1];

                Color nodeColor = (color == 'r') ? Color::RED : Color::BLACK;

                int value = std::stoi(token.substr(0, token.size() - 1));

                std::shared_ptr<Node> newNode = std::make_shared<Node>(Node(value, nodeColor));

                output.addNode(newNode);
            }
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
        lineToken = trim(lineToken);

        std::cout << "Command Line: " << lineToken << std::endl;

        if (!lineToken.empty())
        {
            std::stringstream ss2(lineToken);

            std::string commandToken;

            Command newCommand = Command();
            bool threadRead = false;

            while(std::getline(ss2, commandToken, ','))
            {
                commandToken = trim(commandToken);
                std::cout << "Sub-Command Line: " << commandToken << std::endl;

                if (!threadRead)
                {
                    std::string thread = commandToken.substr(6, commandToken.size() - 6);

                    int threadNum = std::stoi(thread);
                    std::cout << "Setting thread num to: " << thread << std::endl;
                    newCommand.setThreadNum(threadNum);
                    threadRead = true;
                }
                else
                {
                    std::string command = commandToken.substr(0, 6);
                    std::transform(command.begin(), command.end(), command.begin(), ::tolower);

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

                    std::string nodeAffected = commandToken.substr(6, commandToken.size() - 6);

                    std::cout << "Node Affected: " << nodeAffected << std::endl;
                    int node = std::stoi(nodeAffected.substr(1, nodeAffected.size() - 2));
                    newCommand.setNode(node);
                }
            }

            std::cout << "Added Command: " << newCommand.getThreadNum() << " " << newCommand.getNode() << std::endl;
            output.addCommand(newCommand);
        }
    }
}