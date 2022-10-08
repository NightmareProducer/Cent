#ifndef CENT_STMT
#define CENT_STMT

#include "Expressions.hpp"

#include <concepts>

namespace Cent
{
    namespace Constant
    {
        enum class StmtType
        {
            INVALID = -1, 
            PRINT,
            EXPRESSION,
        };
    }

    namespace Type
    {
        struct Stmt
        {
            Constant::StmtType type;
            ExprShrd expr;
        };
    } // namespace Type


    inline Type::Stmt PrintStmt(Type::ExprShrd& p_expr)
    { return {.type = Constant::StmtType::PRINT, .expr = p_expr}; };

    inline Type::Stmt ExprStmt(Type::ExprShrd& p_expr)
    { return {.type = Constant::StmtType::EXPRESSION, .expr = p_expr}; };
} // namespace Cent

#endif