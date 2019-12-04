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
    bool nodesRead = false;
    bool threadsRead = false;
    bool commandsRead = false;

    int testTracker = 1;

    while (std::getline(file, line))
    {
        line = trim(line);

        if (!line.empty())
        {
            if (!nodesRead)
            {
                std::string testLine = line.substr(0, 4);
                std::transform(testLine.begin(), testLine.end(), testLine.begin(), ::tolower);

                // Check to make sure that the first line we read in is NOT the test case header line
                if (testLine.compare("test") != 0)
                {
                    // Since all the nodes are on one line with no return, we can just parse this line and it'll be correct
                    parseNodes(line);
                    nodesRead = true;
                }
                else
                {
                    std::cout << "Test: " << std::to_string(testTracker) << std::endl;
                }
                
            }
            else
            {
                if (!threadsRead)
                {
                    std::string token;

                    token = trim(line);
                    std::string threadType = token.substr(0, 6);
                    std::transform(threadType.begin(), threadType.end(), threadType.begin(), ::tolower);

                    if (threadType.compare("search") == 0)
                    {
                        std::string numThreads = token.substr(15, token.length() - 15);
                        output.setNumReadThreads(std::stoi(numThreads));
                    }
                    else if (threadType.compare("modify") == 0)
                    {
                        std::string numThreads = token.substr(15, token.length() - 15);
                        output.setNumWriteThreads(std::stoi(numThreads));
                        threadsRead = true;
                    }
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

        if (nodesRead && threadsRead && commandsRead)
        {
            // This is a new tree to run, so run it
            RBTreeRun::runTree(output);
            std::cout << std::endl;
            output = FileOutput();
            nodesRead = false;
            threadsRead = false;
            commandsRead = false;
            testTracker++;
        }
    }
    return;
}

void Parser::parseNodes(std::string nodeString)
{
    std::stringstream ss(nodeString);

    std::string token;

    // Most often, nodes are deliminated by commas (as is in the design doc)
    while (std::getline(ss, token, ','))
    {
        // Check for unneeded spaces
        std::stringstream tokenSS(token);
        std::string spaceToken;
        while (std::getline(tokenSS, spaceToken, ' '))
        {
            // We don't terribly care if this is a null node, the balancing will do it for us
            if (spaceToken.compare("f") != 0)
            {
                spaceToken = trim(spaceToken);

                // Check to make sure it's a valid node
                if (isalpha(spaceToken[spaceToken.size() - 1]))
                {
                    char color = spaceToken[spaceToken.size() - 1];

                    Color nodeColor = (color == 'r') ? Color::RED : Color::BLACK;

                    int value = std::stoi(spaceToken.substr(0, spaceToken.size() - 1));

                    std::shared_ptr<Node> newNode = std::make_shared<Node>(Node(value, nodeColor));

                    output.addNode(newNode);
                }
            }
        }
    }
}

void Parser::parseCommands(std::string commandsToParse)
{
    std::stringstream ss(commandsToParse);

    std::string lineToken;

    while (std::getline(ss, lineToken, '|'))
    {
        lineToken = trim(lineToken);

        if (!lineToken.empty())
        {
            Command newCommand = Command();

            std::string nodeAffected = lineToken.substr(6, lineToken.size() - 6);

            int node = std::stoi(nodeAffected.substr(1, nodeAffected.size() - 2));
            newCommand.setNode(node);


            std::string command = lineToken.substr(0, 6);
            std::transform(command.begin(), command.end(), command.begin(), ::tolower);

            if (command.compare("search") == 0)
            {
                newCommand.setCommand(CommandType::SEARCH);
                output.addSearchCommand(newCommand);
            }
            else if (command.compare("insert") == 0)
            {
                newCommand.setCommand(CommandType::INSERT);
                output.addModifyCommand(newCommand);
            }
            else if (command.compare("delete") == 0)
            {
                newCommand.setCommand(CommandType::DELETE);
                output.addModifyCommand(newCommand);
            }
            else
            {
                std::cout << "Error, not a valid command: " << command << std::endl;
            }
        }
    }
}