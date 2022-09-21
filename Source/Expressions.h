#ifndef CENT_EXPR
#define CENT_EXPR

#include "Token.h"

namespace Cent
{
    namespace Constant
    {
        enum class ExpressionType 
        {
            LITERAL, UNARY, BINARY, GROUPING, INVALID
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
        /// @brief Prints the syntax tree of an expression to standard output
        /// @param expr Expression to be printed
        void pretty_print(const Type::ExpressionData* p_expr);

        bool is_valid_expr(const Type::ExpressionData* p_expr);
    } // namespace Tool


    /// @brief Construct an ExpressionData of type Binary
    /// @param p_left The left operand
    /// @param p_op The binary operator
    /// @param p_right The right operand
    /// @return Expression data with type field set to BINARY
    Type::ExpressionData* Binary(Type::ExpressionData *p_left, Type::TokenData *p_op, Type::ExpressionData *p_right) noexcept;
    /// @brief Construct an ExpressionData of type Unary
    /// @param p_op The unary operator
    /// @param p_expr The expression on the right side from the operand
    /// @return Expression data with type field set to UNARY
    Type::ExpressionData* Unary(Type::TokenData *p_op, Type::ExpressionData *p_expr) noexcept;
    /// @brief Construct an ExpressionData of type Grouping
    /// @param p_expr 
    /// @return Expression data with type field set to Grouping
    Type::ExpressionData* Grouping(Type::ExpressionData *p_expr) noexcept;
    /// @brief Construct an ExpressionData of type Literal
    /// @param p_literal The token literal value
    /// @return Expression data with type field set to LITERAL
    Type::ExpressionData* Literal(Type::TokenData *p_literal) noexcept;
    Type::ExpressionData* InvalidExpr(Type::TokenData *p_token) noexcept;
} // namespace Cent

#endif