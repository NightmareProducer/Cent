#include "Parser.hpp"
#include "Handler.hpp"

#include <concepts>
#include <array>


// { Static Definitions
namespace {
    using namespace Cent;
    using namespace Cent::Constant;
    using namespace Cent::Type;
    using namespace Tool;

// { Concepts

    template<typename T> 
    concept TokenTypes = std::same_as<T, Constant::TokenType>;

//// } Concepts

// { Static Variables

    /// @brief current token_list index
    thread_local std::size_t s_index = 0; 
    /// @brief assigned when parse_expr() is called
    thread_local Type::TokenList *s_tokens = nullptr;

//// } Static Variables 


// { Static Function Declarations

    /// @brief Rule: expression -> equality
    /// @param p_token The token to be match with a producer 
    ExprParseRes expression(TokenShrd& p_token) noexcept;
    /// @brief Rule: equality -> comparison ( ( "!=" | "==" ) comparison )*;
    /// @param p_token The token to be match with a producer 
    ExprParseRes equality(TokenShrd& p_token)  noexcept;
    /// @brief Rule: comparison -> term ( ( ">" | ">=" | "<" | "<=" ) term )*;
    /// @param p_token The token to be match with a producer 
    ExprParseRes comparison(TokenShrd& p_token) noexcept;
    /// @brief Rule: term -> factor ( ( "-" | "+" ) factor )*;
    /// @param p_token The token to be match with a producer 
    ExprParseRes term(TokenShrd& p_token) noexcept;
    /// @brief Rule: factor -> unary ( ( "*" | "/" ) unary )*;
    /// @param p_token The token to be match with a producer 
    ExprParseRes factor(TokenShrd& p_token) noexcept;
    /// @brief Rule: unary -> ( "!" | "-" ) unary | primary;
    /// @param p_token The token to be match with a producer 
    ExprParseRes unary(TokenShrd& p_token) noexcept;
    /// @brief Rule: NUMBER | STRING | "true" | "false" | "nil" | "(" EXPRESSION ")";
    /// @param p_token The token to be match with a producer 
    ExprParseRes primary(TokenShrd& p_token) noexcept;

    StmtParseRes statement(TokenShrd& p_token) noexcept;
    StmtParseRes expression_stmt(TokenShrd& p_token) noexcept;
    StmtParseRes print_stmt(TokenShrd& p_token) noexcept;
 
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

    ExprParseRes expression(TokenShrd& p_token) noexcept
    {
        return equality(p_token);
    }

    ExprParseRes equality(TokenShrd& p_token)  noexcept
    {
        using enum Cent::Constant::TokenType;

        return handle(comparison(p_token), [&](auto&& p_res)
        {
            auto &left = p_res.data;
            while(match(current_token(), BANG_EQ, EQUAL_EQ))
            {
                auto op = current_token();
                auto tkn = next_token();
                handle(comparison(tkn), [&](auto p__res)
                {
                    auto &right = p__res.data;
                    p_res.data = Binary(left, op, right);
                });
            }
        });
    }

    ExprParseRes comparison(TokenShrd& p_token) noexcept
    {
        using enum Cent::Constant::TokenType;

        return handle(term(p_token), [&](auto&& p_res) 
        {
            auto& left = p_res.data;
            while(match(current_token(), GREATER, GREATER_EQ, LESS, LESS_EQ))
            {
                auto op = current_token();
                auto tkn = next_token();
                handle(term(tkn), [&](auto&& p__res){
                    auto& right = p__res.data;
                    p_res.data = Binary(left, op, right);
                });
            }   
        });
    }

    ExprParseRes term(TokenShrd& p_token) noexcept
    {
        using enum Cent::Constant::TokenType;

        return handle(factor(p_token), [&](auto&& p_res)
        {
            auto& left = p_res.data;
            while(match(current_token(), MINUS, PLUS))
            {
                auto op = current_token();
                auto tkn = next_token();
                handle(factor(tkn), [&](auto&& p__res) 
                {
                    auto& right = p__res.data;
                    p_res.data = Binary(left, op, right);
                });
            }
        });
    }

    ExprParseRes factor(TokenShrd& p_token) noexcept
    {
        using enum Cent::Constant::TokenType;

        return handle(unary(p_token), [&](auto&& p_res) 
        {
            auto& left = p_res.data;
            while(match(current_token(), SLASH, STAR))
            {
                auto op = current_token();
                auto tkn = next_token();
                handle(unary(tkn), [&](auto&& p__res) 
                {
                    auto& right = p__res.data;
                    p_res.data = Binary(left, op, right);
                });
            }
        });
    }

    ExprParseRes unary(TokenShrd& p_token) noexcept
    {
        using enum Cent::Constant::TokenType;
        if(match(p_token, BANG, MINUS))
        {
            auto tkn = next_token();
            return handle(unary(tkn), [&](auto&& p_res) 
            {
                p_res.data = Unary(p_token, p_res.data);
                next();
            });
        }
        
        return primary(p_token);
    }

    ExprParseRes primary(TokenShrd& p_token) noexcept
    {
        using namespace Cent::Constant;
        using enum Cent::Constant::TokenType;

        auto wrap = [](const ExprShrd& p_expr, ERR p_errcode = ERR::SUCCESS)
        {
            return ExprParseRes{p_expr, p_errcode};
        };

        if(match(p_token, FALSE, TRUE, STRING, INT, FLOAT))
            return wrap((next(), Literal(p_token)));
        
        if(match(p_token, LEFT_PAREN))
        {
            auto tkn = next_token();
            auto res = expression(tkn);

            if(next(); match(current_token(), RIGHT_PAREN))
                return wrap(InvalidExpr(p_token), ERR::MISSING_RIGHT_PAREN);

            return res;
        }

        return wrap(InvalidExpr(p_token), ERR::INVALID_EXPR);
    }


    StmtParseRes statement(TokenShrd& p_token) noexcept
    {
        using enum Constant::TokenType;
        if(!at_end())
        {
            switch(p_token->type)
            {
            case PRINT:
                return print_stmt(p_token);
            }

        }

        return expression_stmt(p_token);
    }

    StmtParseRes print_stmt(TokenShrd& p_token) noexcept
    {
        using enum Constant::TokenType;
        return handle(expression_stmt(p_token), [&](auto&& p_res)
        {
            if(next(); match(current_token(), SEMICOLON))
            {
                p_res.data = PrintStmt(p_res.data.expr);
            }
        });
    }

    StmtParseRes expression_stmt(TokenShrd& p_token) noexcept
    {
        using namespace Constant;
        using enum TokenType;

        StmtParseRes ret;

        handle(expression(p_token), [&](auto&& p_res)
        {
            if(next(); match(current_token(), SEMICOLON))
            {
                ret.data = ExprStmt(p_res.data);
                ret.errcode = ERR::SUCCESS;
            } else
            {
                ret.errcode = ERR::INVALID_EXPR_STMT;
            }
        });

        return ret;
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
    Type::ExprParseRes parse_expr(Type::TokenList& p_tokens) noexcept
    {
        s_index = 0;
        s_tokens = &p_tokens;

        auto tkn = current_token();
        auto ret {expression(tkn)};

        s_tokens = nullptr;

        return ret;
    }


    Type::StmtParseRes parse_stmt(Type::TokenList& p_tokens) noexcept
    {
        s_index = 0;
        s_tokens = &p_tokens;
        auto tkn = current_token();
        auto ret {statement(tkn)};
        s_tokens = nullptr;

        return ret;
    }

    Type::EvalResult evaluate(const Type::ExprShrd& p_expr) noexcept
    {
        using namespace Cent::Constant;
        using namespace Cent::Type;

        auto wrap = [&p_expr](Type::ValueData&& p_val, ERR p_errcode = ERR::SUCCESS){
            return Type::EvalResult {.from = p_expr, .data = p_val, .errcode = p_errcode};
        };

        switch (p_expr->type)
        {
        case ExpressionType::GROUPING:
            return evaluate(std::get<ExprShrd>(p_expr->content));

        case ExpressionType::BINARY: 
            return handle(evaluate(p_expr->left), [&](auto&& p_res) 
            {
                auto& left = p_res.data;
                handle(evaluate(p_expr->right), [&](auto&& p__res) 
                {
                    auto& right = p__res.data;
                    switch (std::get<TokenShrd>(p_expr->content)->type)
                    {
                    case TokenType::STAR:
                        p_res = handle_fail(wrap(left * right), [](auto&& r){ r.errcode = ERR::INVALID_MUL_OPERATION; });
                        return;
                    case TokenType::PLUS:
                        p_res = handle_fail(wrap(left + right), [](auto&& r){ r.errcode = ERR::INVALID_PLUS_OPERATION; });
                        return;
                    case TokenType::MINUS:
                        p_res = handle_fail(wrap(left - right), [](auto&& r){ r.errcode = ERR::INVALID_MINUS_OPERATION; });
                        return;
                    }

                    p_res.errcode = ERR::INVALID_BINARY_EXPR;
                });
            });

        case ExpressionType::UNARY:
            return handle(evaluate(p_expr->right), [&](auto&& p_res) 
            {
                auto& right = p_res.data;

                switch (std::get<TokenShrd>(p_expr->content)->type)
                {
                case TokenType::MINUS:
                    p_res = handle_fail(wrap(-right), [](auto&& r){ r.errcode = ERR::INVALID_NEGATION; });
                    return;
                case TokenType::BANG:
                    p_res = handle_fail(wrap(!right), [](auto&& r){ r.errcode = ERR::INVALID_BANG_OP; });
                    return;
                }

                p_res.errcode = ERR::INVALID_UNARY_EXPR;
            });
        case ExpressionType::LITERAL:
            switch (std::get<TokenShrd>(p_expr->content)->type)
            {
            case TokenType::STRING:
                return wrap(Value(std::get<TokenShrd>(p_expr->content)->literal));
            case TokenType::INT:
                return wrap(Value(std::stoi(std::get<TokenShrd>(p_expr->content)->literal)));
            case TokenType::FLOAT:
                return wrap(Value(std::stof(std::get<TokenShrd>(p_expr->content)->literal)));
            case TokenType::FALSE:
                return wrap(Value(false));
            case TokenType::TRUE:
                return wrap(Value(true));
            }

            return wrap(InvalidValue(), ERR::INVALID_LITERAL);
        }

        return wrap(InvalidValue(), ERR::ILL_FORMED_EXPR);
    }
} // namespace Cent
/// } Header Definitions