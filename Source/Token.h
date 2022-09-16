#ifndef CENT_TOKEN
#define CENT_TOKEN

#include <vector>
#include <string>


namespace Cent
{
    namespace Constant
    {
        enum class TokenType
        {
            // SINGLE CHARACTER TOKENS
            LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
            COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

            // ONE OR TWO CHARACTER TOKENS
            BANG, BANG_EQ,
            EQUAL, EQUAL_EQ,
            GREATER, GREATER_EQ,
            LESS, LESS_EQ,
            COMMENT,

            // LITERALS
            IDENTIFIER, STRING, NUMBER, CHAR,

            // KEYWORDS
            AND, OR, CLASS, IF, ELSE, VAR, FUNC, FOR, WHILE,  
            RETURN, PRINT, SUPER, THIS, TRUE, FALSE, NIL,

            ENOF
        };
    } // namespace Constant


    namespace Type
    {
        struct TokenData 
        {
            int line;
            Constant::TokenType type;
            std::string literal;
            std::string lexeme;
        };
    } // namespace Type


    namespace Tool
    {
        Constant::TokenType reserved(std::string_view word) noexcept;
    }


    Type::TokenData* Token(Constant::TokenType p_type, std::string p_lexeme, std::string p_literal, int p_line);
}

#endif