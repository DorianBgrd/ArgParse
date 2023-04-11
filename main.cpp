#include <iostream>
#include "ArgParse.h"

int main(int argc, char* argv[])
{
    // Create Parser.
    Parser parser;

    // Add flags.
    parser.AddFlag("-t", "--test");
    parser.AddFlag("-n", "--number", 5);
    parser.AddFlag("-str", "--string", -1);

    // Parse command line.
    parser.Parse(argc, argv);

    // Print results.
    std::cout << "found flag test : " << parser.FlagFound("-t") << std::endl;
    std::cout << "Numbers : " << std::endl;
    for (const std::string& v : parser.FlagResults("-n"))
    {
        std::cout << " - " << v << std::endl;
    }
    std::cout << "String : " << parser.FlagResult("-str") << std::endl;
    std::cout << "Arguments : " << std::endl;
    for (const std::string& a : parser.Arguments())
    {
        std::cout << " - " << a << std::endl;
    }

    return 0;
}
