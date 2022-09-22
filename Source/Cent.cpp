#include "Cent.h"

#include <iostream>
#include <string>
#include <array>


using namespace Cent;


int main(int argc, char *argv[]) 
{
    if (argc == 2)
    {
        auto tokens = Scanner::scan(Tool::file2string(argv[1]));
        auto expr = Parser::parse(tokens);
        // auto x = Token(Constant::TokenType::AND, "aa", "aa", 3, 3);
        // expr = Literal(x);
        Tool::pretty_print(expr);
    } else if (argc == 1)
    {
        std::cout << "> ";
        for(std::string line; std::getline(std::cin, line);)
        {
            Scanner::scan(line);
            std::cout << "> ";
        }
    }
    
    return 0;
}