#include "Token.h"

#include <unordered_map>
#include <stdexcept>

using namespace Cent::Constant;


// Static variables
static std::unordered_map 
    <std::string_view, TokenType> s_reserved 
{
    {"and", TokenType::AND},
    {"or", TokenType::OR},
    {"class", TokenType::CLASS},
    {"if", TokenType::IF},
    {"else", TokenType::ELSE},
    {"var", TokenType::VAR},
    {"func", TokenType::FUNC},
    {"for", TokenType::FOR},
    {"while", TokenType::WHILE},
    {"return", TokenType::RETURN},
    {"super", TokenType::SUPER},
    {"this", TokenType::THIS},
    {"print", TokenType::PRINT},
    {"nil", TokenType::NIL},
    {"true", TokenType::TRUE},
    {"false", TokenType::FALSE},
};
//// end static variables

namespace Cent::Tool
{
    Constant::TokenType reserved(std::string_view word) noexcept 
    { 
        try 
        {
            return s_reserved.at(word);
        } 
        catch(const std::out_of_range& e)
        {
            return TokenType::IDENTIFIER;
        }
    }

} // namespace Tool


namespace Cent // External Functions
{
    Type::TokenData* Token(Constant::TokenType p_type, std::string p_lexeme, std::string p_literal, int p_line)
    {
        return new Type::TokenData{p_line, p_type, p_literal, p_lexeme};
    }
} // namespace Cent
