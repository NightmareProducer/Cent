#include "Expressions.h"


namespace Cent
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