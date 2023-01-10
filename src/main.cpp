#include "Parser.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        Parser parser = Parser();
        parser.parse(argv[1]);
    }
    else
    {
        std::cout << "Please provide an input file to read from" << std::endl;
    }
}