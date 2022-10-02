#ifndef CENT_TOKEN
#define CENT_TOKEN

#include <vector>
#include <string>
#include <string_view>
#include <memory>


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
            IDENTIFIER, STRING, CHAR, INT, FLOAT,

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
            int column;
            Constant::TokenType type;
            std::string literal;
            std::string lexeme;
        };


        // Alias

        using TokenPtr = std::unique_ptr<TokenData>;
        using TokenShrd = std::shared_ptr<TokenData>;
        using TokenList = std::vector<TokenShrd>;

        //// Alias
    } // namespace Type


    namespace Tool
    {
        Constant::TokenType reserved(std::string_view word) noexcept;
    }

    Type::TokenPtr Token(Constant::TokenType p_type, std::string p_lexeme, std::string p_literal, int p_line, int p_column);
}

#endif