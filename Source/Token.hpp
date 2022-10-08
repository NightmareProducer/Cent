#ifndef CENT_TOKEN
#define CENT_TOKEN

#include <vector>
#include <string>
#include <string_view>
#include <memory>
#include <concepts>
#include <type_traits>


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


        using TokenPtr = std::unique_ptr<TokenData>;
        using TokenShrd = std::shared_ptr<TokenData>;
        using TokenList = std::vector<TokenShrd>;
    } // namespace Type

    namespace Tool
    {
        using namespace Type;

        using LRefTkn = std::add_lvalue_reference_t<TokenShrd>;
    }

    
    namespace Concept
    {
        using namespace Type;

        template<typename T>
        concept isTokenShrd = std::same_as<std::decay_t<T>, TokenShrd>;
    } // namespace Concept


    namespace Tool
    {
        Constant::TokenType reserved(std::string_view word) noexcept;
    }

    Type::TokenPtr Token(Constant::TokenType p_type, std::string p_lexeme, std::string p_literal, int p_line, int p_column);
}

#endif