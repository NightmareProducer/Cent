#include "Expressions.h"

#include <iostream>


namespace Cent::Tool
{
    void pretty_print(const Type::ExprShrd& p_expr)
    {
        using namespace Cent::Constant;
        if (!p_expr) return;

        switch(p_expr->type)
        {
            case ExpressionType::LITERAL:
                std::cout << p_expr->content_token->lexeme;
                break;
            case ExpressionType::UNARY:
                std::cout << "(" << p_expr->content_token->lexeme;
                pretty_print(p_expr->right);
                std::cout << ")";
                break;
            case ExpressionType::GROUPING:
                std::cout << "(";
                pretty_print(p_expr->content_expr);
                std::cout << ")";
                break;
            case ExpressionType::BINARY:
                std::cout << "(" << p_expr->content_token->lexeme;
                pretty_print(p_expr->left);
                pretty_print(p_expr->right);
                std::cout << ")";
                break;
        }
    }

    bool is_valid_expr(const Type::ExprShrd& p_expr)
    {
        return p_expr->type != Constant::ExpressionType::INVALID;
    }
} // namespace Tool


namespace Cent // External Functions
{
    Type::ExprPtr Binary(Type::ExprShrd p_left, Type::TokenShrd p_op, Type::ExprShrd p_right) noexcept
    {
        return std::unique_ptr<Type::ExpressionData>(
            new Type::ExpressionData{Constant::ExpressionType::BINARY, p_op, nullptr, p_left, p_right});
    }

    Type::ExprPtr Unary(Type::TokenShrd p_op, Type::ExprShrd p_expr) noexcept
    {
        return std::unique_ptr<Type::ExpressionData>(
            new Type::ExpressionData{Constant::ExpressionType::UNARY, p_op, nullptr, nullptr, p_expr});
    }

    Type::ExprPtr Grouping(Type::ExprShrd p_expr) noexcept
    {
        return std::unique_ptr<Type::ExpressionData>(
            new Type::ExpressionData{Constant::ExpressionType::GROUPING, nullptr, p_expr});
    }

    Type::ExprPtr Literal(Type::TokenShrd p_literal) noexcept
    {
        return std::unique_ptr<Type::ExpressionData>(
            new Type::ExpressionData{Constant::ExpressionType::LITERAL, p_literal});
    }

    Type::ExprPtr InvalidExpr(Type::TokenShrd p_token) noexcept
    {
        return std::unique_ptr<Type::ExpressionData>(
            new Type::ExpressionData{Constant::ExpressionType::INVALID, p_token});
    }
} // namespace Cent