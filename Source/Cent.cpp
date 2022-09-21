#include "Cent.h"

#include <iostream>
#include <string>
#include <array>


using namespace Cent;


int main(int argc, char *argv[]) 
{
    Tool::pretty_print(
        Binary(
            Unary(
                Token(Constant::TokenType::MINUS, "-", "", 1),
                Literal(Token(Constant::TokenType::NUMBER, "\"123\"", "123", 1))
            ),
            Token(Constant::TokenType::STAR, "*", "", 1),
            Grouping(
                Literal(Token(Constant::TokenType::NUMBER, "\"4444\"", "4444", 1))
            )
        )
    );


    if (argc == 2)
    {
       auto Tokens = Scanner::scan(Tool::file2string(argv[1]));

        for(auto token : Tokens)
            delete token;
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