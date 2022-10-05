#ifndef CENT_ERROR
#define CENT_ERROR

#include "Expressions.h"

#include <string>
#include <iostream>

namespace Cent::Constant
{
    enum class ERR {
        INVALID_OPERATION,

        INVALID_BINARY_EXPR,
        INVALID_UNARY_EXPR,
        ILL_FORMED_EXPR,

        INVALID_LITERAL,
    };
}

namespace Cent::Type
{
    struct SyntaxError 
    {
        int line;
        int column;
        std::string reason;
    };

    struct EvalErr
    {
        ExprShrd from;
        Constant::ERR code;
    };
}

namespace Cent::Error
{
    /// @brief Enqueue an error into the error queue.
    /// @param p_line   The line where the error occured
    /// @param p_column The column where the error occured
    /// @param p_reason The reasoning behind the error.
    /// @param p_phase  Which phased the error occured 
    void syntax(int p_line, int p_column, std::string p_reason) noexcept;
} // namespace Error
#endif