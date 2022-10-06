#include "Parser.h"
#include "Error.h"
#include "Handler.h"

#include <concepts>
#include <array>
#include <variant>
#include <type_traits>


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
    ParseResult expression(TokenShrd p_token) noexcept;
    /// @brief Rule: equality -> comparison ( ( "!=" | "==" ) comparison )*;
    /// @param p_token The token to be match with a producer 
    ParseResult equality(TokenShrd p_token)  noexcept;
    /// @brief Rule: comparison -> term ( ( ">" | ">=" | "<" | "<=" ) term )*;
    /// @param p_token The token to be match with a producer 
    ParseResult comparison(TokenShrd p_token) noexcept;
    /// @brief Rule: term -> factor ( ( "-" | "+" ) factor )*;
    /// @param p_token The token to be match with a producer 
    ParseResult term(TokenShrd p_token) noexcept;
    /// @brief Rule: factor -> unary ( ( "*" | "/" ) unary )*;
    /// @param p_token The token to be match with a producer 
    ParseResult factor(TokenShrd p_token) noexcept;
    /// @brief Rule: unary -> ( "!" | "-" ) unary | primary;
    /// @param p_token The token to be match with a producer 
    ParseResult unary(TokenShrd p_token) noexcept;
    /// @brief Rule: NUMBER | STRING | "true" | "false" | "nil" | "(" EXPRESSION ")";
    /// @param p_token The token to be match with a producer 
    ParseResult primary(TokenShrd p_token) noexcept;
 
    /// @brief Check wether the token matches any of the specified token types.
    /// @tparam ...T Enum TokenType
    /// @param p_token Token to be compared with
    /// @param ...p_types Token types to be matched with
    /// @return true if token matched any of the specified type, false otherwise
    template<TokenTypes... T>
    bool match(const TokenShrd& p_token, T... p_types);
    /// @brief Checks whether we are at the end of the token_list
    /// @return true if s_index >= s_tokens->size(), false otherwise
    inline bool at_end();
    /// @brief Increments the index
    inline void next();
    /// @brief Returns the token at the current index
    TokenShrd current_token();
    /// @brief Increments the index and returns the token
    TokenShrd next_token();

    //// } Static Functions Declarations 


    // { Static Function Definitions
    ParseResult wrap(const ExprShrd& p_expr, ERR p_errcode)
    {
        return {p_expr, p_errcode};
    }

    ParseResult wrap(const ExprShrd& p_expr)
    {
        return {p_expr, ERR::SUCCESS};
    }


    ParseResult expression(TokenShrd p_token) noexcept
    {
        return equality(p_token);
    }

    ParseResult equality(TokenShrd p_token)  noexcept
    {
        using enum Cent::Constant::TokenType;

        return handle(comparison(p_token), [&](auto&& p_left)
        {
            while(match(current_token(), BANG_EQ, EQUAL_EQ))
            {
                auto op = current_token();
                handle(comparison(next_token()), [&](auto p_right)
                {
                    // Binary(p_left, op, p_right);
                    // p_left = Binary(p_left, op, p_right);
                    // auto x = Binary(p_left, op, p_right);
                });
            }
        });
    }

    ParseResult comparison(TokenShrd p_token) noexcept
    {
        using enum Cent::Constant::TokenType;

        return handle(term(p_token), [&](auto&& p_res) 
        {
            // while(match(current_token(), GREATER, GREATER_EQ, LESS, LESS_EQ))
            // {
            //     auto op = current_token(); 
            //     handle(term(next_token()), [&](auto&& p__res){
            //         p_res.expr = Binary(p_res.expr, op, p__res.expr);
            //     });
            // }   
        });
    }

    ParseResult term(TokenShrd p_token) noexcept
    {
        using enum Cent::Constant::TokenType;

        return handle(factor(p_token), [&](auto&& p_left)
        {
            while(match(current_token(), MINUS, PLUS))
            {
                auto op = current_token();
                handle(factor(next_token()), [&](auto&& p_right) 
                {
                    p_left = Binary(p_left, op, p_right);
                });
            }
        });
    }

    ParseResult factor(TokenShrd p_token) noexcept
    {
        using enum Cent::Constant::TokenType;

        return handle(unary(p_token), [&](auto&& p_left) 
        {
            while(match(current_token(), SLASH, STAR))
            {
                auto op = current_token();
                handle(unary(next_token()), [&](auto&& p_right) 
                {
                    p_left = Binary(p_left, op, p_right);
                });
            }
        });
    }

    ParseResult unary(TokenShrd p_token) noexcept
    {
        using enum Cent::Constant::TokenType;
        if(match(p_token, BANG, MINUS))
        {
            return handle(unary(next_token()), [&](auto&& expr) 
            {
                expr = Unary(p_token, expr);
                next();
            });
        }
        
        return primary(p_token);
    }

    ParseResult primary(TokenShrd p_token) noexcept
    {
        using namespace Cent::Constant;
        using enum Cent::Constant::TokenType;

        if(match(p_token, FALSE, TRUE, STRING, INT, FLOAT))
            return wrap((next(), Literal(p_token)));
        
        if(match(p_token, LEFT_PAREN))
        {
            auto res = expression(next_token());

            if(next(); match(current_token(), RIGHT_PAREN))
                return wrap(InvalidExpr(p_token), ERR::MISSING_RIGHT_PAREN);

            return res;
        }

        return wrap(InvalidExpr(p_token), ERR::INVALID_EXPR);
    }

    template<TokenTypes... T>
    bool match(const TokenShrd& p_token, T... p_types)
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

    TokenShrd current_token() 
    { 
        if(!at_end()) return (*s_tokens)[s_index];
        return nullptr;
    }

    TokenShrd next_token() { 
        if(!at_end()) return (*s_tokens)[++s_index]; 
        return nullptr;
    }

    
//// } Static Function Definitions 
}
//// } Static Definitions 


// { Header Definitions
namespace Cent::Parser
{
    Type::ParseResult parse(Type::TokenList p_tokens) noexcept
    {
        s_index = 0;
        s_tokens = &p_tokens;

        auto ret {expression(current_token())};

        s_tokens = nullptr;

        return ret;
    }

    Type::ValueData evaluate(const Type::ExprShrd& p_expr) noexcept
    {
        using namespace Cent::Constant;
        using namespace Cent::Type;

        // set the from expression when handling invalid value
        auto wrap = [&p_expr](Type::ValueData&& p_val){
            if(p_val.type == ValueType::INVALID)
                std::get<Type::EvalErr>(p_val.content).from = p_expr;

            return p_val;
        };

        switch (p_expr->type)
        {
        case ExpressionType::GROUPING:
            return evaluate(std::get<ExprShrd>(p_expr->content));

        case ExpressionType::BINARY: 
        {
            auto left = evaluate(p_expr->left);
            auto right = evaluate(p_expr->right);

            switch (std::get<TokenShrd>(p_expr->content)->type)
            {
            case TokenType::STAR:
                return wrap(left * right);
            case TokenType::PLUS:
                return wrap(left + right);
            case TokenType::MINUS:
                return wrap(left - right);
            }
        }

            return ValueErr(p_expr, ERR::INVALID_BINARY_EXPR);
        case ExpressionType::UNARY:
        {
            auto right = evaluate(p_expr->right);
            switch (std::get<TokenShrd>(p_expr->content)->type)
            {
            case TokenType::MINUS:
                return wrap(-right);
            case TokenType::BANG:
                return wrap(!right);
            }
        }

            return ValueErr(p_expr, ERR::INVALID_UNARY_EXPR);
        case ExpressionType::LITERAL:
            switch (std::get<TokenShrd>(p_expr->content)->type)
            {
            case TokenType::STRING:
                return Value(std::get<TokenShrd>(p_expr->content)->literal);
            case TokenType::INT:
                return Value(std::stoi(std::get<TokenShrd>(p_expr->content)->literal));
            case TokenType::FLOAT:
                return Value(std::stof(std::get<TokenShrd>(p_expr->content)->literal));
            case TokenType::FALSE:
                return Value(false);
            case TokenType::TRUE:
                return Value(true);
            }

            return ValueErr(p_expr, ERR::INVALID_LITERAL);
        }

        return ValueErr(p_expr, ERR::ILL_FORMED_EXPR);
    }
} // namespace Cent
/// } Header Definitions