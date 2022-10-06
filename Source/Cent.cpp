#include "Cent.h"

#include <iostream>


using namespace Cent;


int main(int argc, char *argv[]) 
{

    if (argc == 2)
    {
        auto content = Tool::file2string(argv[1]);

        auto tokens = Scanner::scan(content);

        handle(Parser::parse(tokens), [](auto&& p_res) 
        {
            auto& expr = p_res.data;
            Tool::pretty_print(expr);

            // handle(Parser::evaluate(parse_result.expr), [](auto &&v) 
            // {
            //     //handle value on success
            // });
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