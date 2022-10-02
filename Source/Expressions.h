#ifndef CENT_EXPR
#define CENT_EXPR

#include "Token.h"

#include <memory>

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
            TokenShared content_token = nullptr;
            std::shared_ptr<ExpressionData> content_expr = nullptr;
            std::shared_ptr<ExpressionData> left = nullptr;
            std::shared_ptr<ExpressionData> right = nullptr;
        };

        using ExpressionPtr = std::unique_ptr<ExpressionData>;
        using ExpressionShared = std::shared_ptr<ExpressionData>;
    } // namespace Type


    namespace Tool
    {
        /// @brief Prints the syntax tree of an expression to standard output
        /// @param expr Expression to be printed
        void pretty_print(const Type::ExpressionShared& p_expr);

        bool is_valid_expr(const Type::ExpressionShared& p_expr);
    } // namespace Tool


    /// @brief Construct an ExpressionData of type Binary
    /// @param p_left The left operand
    /// @param p_op The binary operator
    /// @param p_right The right operand
    /// @return Expression data with type field set to BINARY
    Type::ExpressionPtr Binary(Type::ExpressionShared p_left, Type::TokenShared p_op, Type::ExpressionShared p_right) noexcept;

    /// @brief Construct an ExpressionData of type Unary
    /// @param p_op The unary operator
    /// @param p_expr The expression on the right side from the operand
    /// @return Expression data with type field set to UNARY
    Type::ExpressionPtr Unary(Type::TokenShared p_op, Type::ExpressionShared p_expr) noexcept;

    /// @brief Construct an ExpressionData of type Grouping
    /// @param p_expr 
    /// @return Expression data with type field set to Grouping
    Type::ExpressionPtr Grouping(Type::ExpressionShared p_expr) noexcept;

    /// @brief Construct an ExpressionData of type Literal
    /// @param p_literal The token literal value
    /// @return Expression data with type field set to LITERAL
    Type::ExpressionPtr Literal(Type::TokenShared p_literal) noexcept;

    Type::ExpressionPtr InvalidExpr(Type::TokenShared p_token) noexcept;
} // namespace Cent

#endif