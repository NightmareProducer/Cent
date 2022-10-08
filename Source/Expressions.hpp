#ifndef CENT_EXPR
#define CENT_EXPR

#include "Token.hpp"

#include <memory>
#include <variant>
#include <concepts>
#include <type_traits>

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
            std::variant<TokenShrd, std::shared_ptr<ExpressionData>> content;
            std::shared_ptr<ExpressionData> left = nullptr;
            std::shared_ptr<ExpressionData> right = nullptr;
        };

        using ExprPtr = std::unique_ptr<ExpressionData>;
        using ExprShrd = std::shared_ptr<ExpressionData>;
    } // namespace Type

    
    namespace Concept
    {
        using namespace Type;

        template<typename T>
        concept isExprShrd = std::same_as<std::decay_t<T>, ExprShrd>;
    } // namespace Concept


    namespace Tool
    {
        /// @brief Prints the syntax tree of an expression to standard output
        /// @param expr Expression to be printed
        void pretty_print(const Type::ExprShrd& p_expr);

        bool is_valid_expr(const Type::ExprShrd& p_expr);

        inline bool is_valid(const Type::ExprShrd& p_value)
        {
            return is_valid_expr(p_value);
        }
    } // namespace Tool


    /// @brief Construct an ExpressionData of type Binary
    /// @param p_left The left operand
    /// @param p_op The binary operator
    /// @param p_right The right operand
    /// @return Expression data with type field set to BINARY
    Type::ExprPtr Binary(Type::ExprShrd& p_left, Type::TokenShrd& p_op, Type::ExprShrd& p_right) noexcept;

    /// @brief Construct an ExpressionData of type Unary
    /// @param p_op The unary operator
    /// @param p_expr The expression on the right side from the operand
    /// @return Expression data with type field set to UNARY
    Type::ExprPtr Unary(Type::TokenShrd& p_op, Type::ExprShrd& p_expr) noexcept;

    /// @brief Construct an ExpressionData of type Grouping
    /// @param p_expr 
    /// @return Expression data with type field set to Grouping
    Type::ExprPtr Grouping(Type::ExprShrd& p_expr) noexcept;

    /// @brief Construct an ExpressionData of type Literal
    /// @param p_literal The token literal value
    /// @return Expression data with type field set to LITERAL
    Type::ExprPtr Literal(Type::TokenShrd& p_literal) noexcept;

    Type::ExprPtr InvalidExpr(Type::TokenShrd& p_token) noexcept;
} // namespace Cent

#endif