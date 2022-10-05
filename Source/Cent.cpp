#include "Cent.h"

#include <iostream>
#include <string>
#include <array>
#include <concepts>
#include <functional>


using namespace Cent;

int main(int argc, char *argv[]) 
{
    if (argc == 2)
    {
        auto content = Tool::file2string(argv[1]);

        auto tokens = Scanner::scan(content);
        // auto expr = Parser::parse(tokens);

        handle(Parser::parse(tokens), [](auto &&expr) 
        {
            Tool::pretty_print(expr);

            handle(Parser::evaluate_expr(expr), [](auto &&v) 
            {
                //handle value on success
            });
        });
        
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