#include "Parser.h"
#include "Expressions.h"
#include "Error.h"

#include <concepts>
#include <array>


// { Static Definitions
namespace {
    using namespace Cent;
    using namespace Cent::Constant;
    using namespace Cent::Type;

    // { Concepts

    template<typename T> 
    concept TokenTypes = std::same_as<T, Constant::TokenType>;

    //// } Concepts

    // { Static Variables

    /// @brief current token_list index
    thread_local std::size_t s_index = 0; 
    /// @brief assigned when parse() is called
    thread_local Type::TokenList *s_tokens = nullptr;

    //// } Static Variables 


    // { Static Function Declarations

    /// @brief Rule: expression -> equality
    /// @param p_token The token to be match with a producer 
    ExpressionData* expression(TokenData* p_token) noexcept;
    /// @brief Rule: equality -> comparison ( ( "!=" | "==" ) comparison )*;
    /// @param p_token The token to be match with a producer 
    ExpressionData* equality(TokenData* p_token)  noexcept;
    /// @brief Rule: comparison -> term ( ( ">" | ">=" | "<" | "<=" ) term )*;
    /// @param p_token The token to be match with a producer 
    ExpressionData* comparison(TokenData* p_token) noexcept;
    /// @brief Rule: term -> factor ( ( "-" | "+" ) factor )*;
    /// @param p_token The token to be match with a producer 
    ExpressionData* term(TokenData* p_token) noexcept;
    /// @brief Rule: factor -> unary ( ( "*" | "/" ) unary )*;
    /// @param p_token The token to be match with a producer 
    ExpressionData* factor(TokenData* p_token) noexcept;
    /// @brief Rule: unary -> ( "!" | "-" ) unary | primary;
    /// @param p_token The token to be match with a producer 
    ExpressionData* unary(TokenData* p_token) noexcept;
    /// @brief Rule: NUMBER | STRING | "true" | "false" | "nil" | "(" EXPRESSION ")";
    /// @param p_token The token to be match with a producer 
    ExpressionData* primary(TokenData* p_token) noexcept;
 
    /// @brief Check wether the token matches any of the specified token types.
    /// @tparam ...T Enum TokenType
    /// @param p_token Token to be compared with
    /// @param ...p_types Token types to be matched with
    /// @return true if token matched any of the specified type, false otherwise
    template<TokenTypes... T>
    bool match(TokenData &p_token, T... p_types);
    /// @brief Checks whether we are at the end of the token_list
    /// @return true if s_index >= s_tokens->size(), false otherwise
    inline bool at_end();
    /// @brief Increments the index
    inline void next();
    /// @brief Returns the token at the current index
    inline TokenData* current_token();
    /// @brief Increments the index and returns the token
    inline TokenData* next_token();

    //// } Static Functions Declarations 


    // { Static Function Definitions
    ExpressionData* expression(TokenData* p_token) noexcept
    {
        return equality(p_token);
    }

    ExpressionData* equality(TokenData* p_token)  noexcept
    {
        using enum Cent::Constant::TokenType;

        auto expr = comparison(p_token);

        while(match(current_token(), BANG_EQ, EQUAL_EQ))
        {
            auto op = current_token();
            auto right = comparison(next_token());
            expr = Binary(expr, op, right);
        }

        return expr;
    }

    ExpressionData* comparison(Type::TokenData* p_token) noexcept
    {
        using enum Cent::Constant::TokenType;

        auto expr = term(p_token);

        while(match(current_token(), GREATER, GREATER_EQ, LESS, LESS_EQ))
        {
            auto op = current_token(); 
            auto right = term(next_token());
            expr = Binary(expr, op, right);
        }

        return expr;
    }

    ExpressionData* term(Type::TokenData* p_token) noexcept
    {
        using enum Cent::Constant::TokenType;

        auto expr = factor(p_token);

        while(match(current_token(), MINUS, PLUS))
        {
            auto op = current_token();
            auto right = factor(next_token());
            expr = Binary(expr, op, right);
        }

        return expr;
    }

    ExpressionData* factor(Type::TokenData* p_token) noexcept
    {
        using enum Cent::Constant::TokenType;

        auto expr = unary(p_token);

        while(match(current_token(), SLASH, STAR))
        {
            auto op = current_token();
            auto right = unary(next_token());
            expr = Binary(expr, op, right);
        }

        return expr;
    }

    ExpressionData* unary(Type::TokenData* p_token) noexcept
    {
        using enum Cent::Constant::TokenType;
        if(match(p_token, BANG, MINUS))
        {
            auto op = p_token;
            auto right = unary(p_token);
            return (next(), Unary(op, right));
        }
        
        return primary(p_token);
    }

    ExpressionData* primary(Type::TokenData* p_token) noexcept
    {
        using enum Cent::Constant::TokenType;

        if(match(p_token, FALSE, TRUE, STRING, NUMBER))
            return (next(), Literal(p_token));
        
        if(match(p_token, LEFT_PAREN))
        {
            auto expr = expression(next_token());

            if(next(); match(current_token(), RIGHT_PAREN))
                Cent::Error::enqueue(current_token()->line, 
                                     current_token()->column, 
                                     "Expect ')' after expression.", 
                                     Phase::PARSING);

            return Grouping(expr);
        }
    }

    template<TokenTypes... T>
    bool match(Type::TokenData *p_token, T... p_types)
    {
        if(at_end()) 
            return false;

        for(TokenType type : std::array{p_types...})
            if (p_token->type == type)
                return true;
        
        return false;
    }

    inline bool at_end()
    {
        return s_index >= s_tokens->size();
    }

    inline void next() {++s_index;} 

    inline TokenData* current_token() { return (*s_tokens)[s_index]; }
    inline TokenData* next_token() { return (*s_tokens)[++s_index]; }
//// } Static Function Definitions 
}
//// } Static Definitions 


// { Header Definitions
namespace Cent
{
    void Parser::parse(Type::TokenList &p_tokens) noexcept
    {
        s_index = 0;
        s_tokens = &p_tokens;

        if(p_tokens.empty()) return;

        expression(current_token());
    }
} // namespace Cent
/// } Headere Definitions