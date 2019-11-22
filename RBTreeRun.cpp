#include "Node.hpp"
#include "RBTree.hpp"
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
}