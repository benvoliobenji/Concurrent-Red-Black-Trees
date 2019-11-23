#include "Node.hpp"
#include "RBTree.hpp"
#include "Parser.hpp"
#include "FileOutput.hpp"

#include <fstream>
#include <iostream>
#include <pthread.h>

void threadRun(std::vector<Command> commands, RBTree& rbtree)
{
    for (auto i = commands.begin(); i != commands.end(); ++i)
    {
        Command command = (*i);
        switch (command.getCommand())
        {
        case CommandType::SEARCH:
            rbtree.search(command.getNode());
            break;
        case CommandType::INSERT:
            rbtree.insert(command.getNode());
            break;
        case CommandType::DELETE:
            rbtree.deleteNode(command.getNode());
            break;
        default:
            std::cout << "Not a valid command" << std::endl;
            break;
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    // RBTree tree = RBTree();

    // Node *node = new Node(7, Color::BLACK);
    // tree.insertNode(node);

    // node = new Node(3, Color::BLACK);
    // tree.insertNode(node);

    // node = new Node(18, Color::RED);
    // tree.insertNode(node);

    // node = new Node(10, Color::BLACK);
    // tree.insertNode(node);

    // node = new Node(8, Color::RED);
    // tree.insertNode(node);

    // node = new Node(11, Color::RED);
    // tree.insertNode(node);

    // node = new Node(22, Color::BLACK);
    // tree.insertNode(node);

    // node = new Node(26, Color::RED);
    // tree.insertNode(node);

    // tree.print();

    // bool found = tree.search(7);
    // std::cout << "Finding node 7: " << found << std::endl;

    // tree.deleteNode(7);

    // found = tree.search(7);
    // std::cout << "Finding node 7: " << found << std::endl;

    // tree.print();

    Parser parser = Parser();
    FileOutput output = parser.parse(argv[1]);
    RBTree fileTree = RBTree();

    std::vector<Node *> nodes = output.getNodes();

    for (auto i = nodes.begin(); i != nodes.end(); ++i)
    {
        std::cout << "Node: " << (*i)->getKey() << std::endl;
        fileTree.insertNode(*i);
    }

    std::vector<std::vector<Command>> commands = output.getCommands();

    std::cout << "Size of the commands: " << commands.size() << std::endl;

    for (auto i = commands.begin(); i != commands.end(); ++i)
    {
        std::vector<Command> threadCommand = *i;
        std::cout << "Size of this thread's number of commands: " << threadCommand.size() << std::endl;
        for (auto j = threadCommand.begin(); j != threadCommand.end(); ++j)
        {
            std::cout << "Thread: " << (*j).getThreadNum() << "\tNode: " << (*j).getNode() << std::endl;
        }
    }
}