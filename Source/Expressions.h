#ifndef CENT_EXPR
#define CENT_EXPR

#include "Token.h"

namespace Cent
{
    namespace Constant
    {
        enum class ExpressionType 
        {
            LITERAL, UNARY, BINARY, GROUPING
        };
    } // namespace Constant


    namespace Type
    {
        struct ExpressionData
        {
            Constant::ExpressionType type;
            TokenData *content_token = nullptr;
            ExpressionData *content_expr = nullptr;
            ExpressionData *left = nullptr;
            ExpressionData *right = nullptr;
        };
    } // namespace Type


    namespace Tool
    {
        void pretty_print(const Type::ExpressionData* expr);
    } // namespace Tool


    Type::ExpressionData* Binary(Type::ExpressionData *p_left, Type::TokenData *p_op, Type::ExpressionData *p_right) noexcept;
    Type::ExpressionData* Unary(Type::TokenData *p_op, Type::ExpressionData *p_expr) noexcept;
    Type::ExpressionData* Grouping(Type::ExpressionData *p_expr) noexcept;
    Type::ExpressionData* Literal(Type::TokenData *p_literal) noexcept;
} // namespace Cent

#endif