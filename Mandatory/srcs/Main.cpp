#include <iostream>
#include <string>
#include "Parser.hpp"
#include "Equation.hpp"

int main(int argc, char **argv){
    std::string input;

    if (argc == 1){
        std::cout << "Enter Equation: " ;
        std::getline(std::cin, input);
    }
    else if (argc == 2)
        input = argv[1];
    else{
        std::cerr << "Usage: ./computor 'equation'" << std::endl;
        return 1;
    }

    Parser parser(input);
    if (!parser.parse())
    {
        std::cerr << "Error: Invalid equation." << std::endl;
        return 1;
    }

    Equation equation(parser.getTerms());
    equation.solve();

    return 0;
}