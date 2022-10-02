#include "Expressions.h"

#include <iostream>


namespace Cent::Tool
{
    void pretty_print(const Type::ExpressionShared& p_expr)
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

    bool is_valid_expr(const Type::ExpressionShared& p_expr)
    {
        return p_expr->type != Constant::ExpressionType::INVALID;
    }
} // namespace Tool


namespace Cent // External Functions
{
    Type::ExpressionPtr Binary(Type::ExpressionShared p_left, Type::TokenShared p_op, Type::ExpressionShared p_right) noexcept
    {
        return std::unique_ptr<Type::ExpressionData>(
            new Type::ExpressionData{Constant::ExpressionType::BINARY, p_op, nullptr, p_left, p_right});
    }

    Type::ExpressionPtr Unary(Type::TokenShared p_op, Type::ExpressionShared p_expr) noexcept
    {
        return std::unique_ptr<Type::ExpressionData>(
            new Type::ExpressionData{Constant::ExpressionType::UNARY, p_op, nullptr, nullptr, p_expr});
    }

    Type::ExpressionPtr Grouping(Type::ExpressionShared p_expr) noexcept
    {
        return std::unique_ptr<Type::ExpressionData>(
            new Type::ExpressionData{Constant::ExpressionType::GROUPING, nullptr, p_expr});
    }

    Type::ExpressionPtr Literal(Type::TokenShared p_literal) noexcept
    {
        return std::unique_ptr<Type::ExpressionData>(
            new Type::ExpressionData{Constant::ExpressionType::LITERAL, p_literal});
    }

    Type::ExpressionPtr InvalidExpr(Type::TokenShared p_token) noexcept
    {
        return std::unique_ptr<Type::ExpressionData>(
            new Type::ExpressionData{Constant::ExpressionType::INVALID, p_token});
    }
} // namespace Cent