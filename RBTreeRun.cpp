#include "Node.hpp"
#include "RBTree.hpp"
#include "Parser.hpp"
#include "FileOutput.hpp"

#include <fstream>
#include <iostream>

int main(int argc, char *argv[])
{
    RBTree tree = RBTree();

    Node *node = new Node(7, Color::BLACK);
    tree.insertNode(node);

    node = new Node(3, Color::BLACK);
    tree.insertNode(node);

    node = new Node(18, Color::RED);
    tree.insertNode(node);

    node = new Node(10, Color::BLACK);
    tree.insertNode(node);

    node = new Node(8, Color::RED);
    tree.insertNode(node);

    node = new Node(11, Color::RED);
    tree.insertNode(node);

    node = new Node(22, Color::BLACK);
    tree.insertNode(node);

    node = new Node(26, Color::RED);
    tree.insertNode(node);

    tree.print();

    bool found = tree.search(7);
    std::cout << "Finding node 7: " << found << std::endl;

    tree.deleteNode(7);

    found = tree.search(7);
    std::cout << "Finding node 7: " << found << std::endl;

    tree.print();

    Parser parser = Parser();
    FileOutput output = parser.parse(argv[1]);

    std::vector<Node *> nodes = output.getNodes();

    for (auto i = nodes.begin(); i != nodes.end(); ++i)
    {
        std::cout << "Node: " << (*i)->getKey() << std::endl;
    }

    std::vector<std::vector<Command>> commands = output.getCommands();
    for (auto i = commands.begin(); i != commands.end(); ++i)
    {
        std::vector<Command> threadCommand = *i;
        for (auto j = threadCommand.begin(); j != threadCommand.end(); ++j)
        {
            std::cout << "Thread: " << (*j).getThreadNum() << "\tNode: " << (*j).getNode() << std::endl;
        }
    }
}