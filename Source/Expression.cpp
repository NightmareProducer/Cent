#include "Expressions.h"

#include <iostream>


namespace Cent::Tool
{
    void pretty_print(const Type::ExpressionData* expr)
    {
        using namespace Cent::Constant;
        if (!expr) return;

        switch(expr->type)
        {
            case ExpressionType::LITERAL:
                std::cout << expr->content_token->lexeme;
                break;
            case ExpressionType::UNARY:
                std::cout << "(" << expr->content_token->lexeme;
                pretty_print(expr->right);
                std::cout << ")";
                break;
            case ExpressionType::GROUPING:
                std::cout << "(";
                pretty_print(expr->content_expr);
                std::cout << ")";
                break;
            case ExpressionType::BINARY:
                std::cout << "(" << expr->content_token->lexeme;
                pretty_print(expr->left);
                pretty_print(expr->right);
                std::cout << ")";
                break;
        }
    }
} // namespace Tool


namespace Cent // External Functions
{
    Type::ExpressionData* Binary(Type::ExpressionData *p_left, Type::TokenData *p_op, Type::ExpressionData *p_right) noexcept
    {
        return new Type::ExpressionData{Constant::ExpressionType::BINARY, p_op, nullptr, p_left, p_right};
    }

    Type::ExpressionData* Unary(Type::TokenData *p_op, Type::ExpressionData *p_expr) noexcept
    {
        return new Type::ExpressionData{Constant::ExpressionType::UNARY, p_op, nullptr, nullptr, p_expr};
    }

    Type::ExpressionData* Grouping(Type::ExpressionData *p_expr) noexcept
    {
        return new Type::ExpressionData{Constant::ExpressionType::GROUPING, nullptr, p_expr};
    }

    Type::ExpressionData* Literal(Type::TokenData *p_literal) noexcept
    {
        return new Type::ExpressionData{Constant::ExpressionType::LITERAL, p_literal};
    }
} // namespace Cent