#include "Cent.h"

#include <iostream>
#include <string>
#include <array>


using namespace Cent;


int main(int argc, char *argv[]) 
{
    auto str1 = String("hello");
    auto str2 = String("world");
    auto str3 = str1 + str2;
    std::cout << str3.value_str << std::endl;

    if (argc == 2)
    {
        auto content = Tool::file2string(argv[1]);

        auto tokens = Scanner::scan(content);
        auto expr = Parser::parse(tokens);
        if(Tool::is_valid_expr(expr))
            Tool::pretty_print(expr);
        else
        {
            std::cerr << "Invalid Expression" << std::endl;
            std::cerr << content << std::endl;
        }
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