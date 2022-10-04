#ifndef CENT_ERROR
#define CENT_ERROR

#include "Expressions.h"

#include <string>

namespace Cent
{
    namespace Constant
    {
        enum class Phase { SCANNING, PARSING, EVAL};

        enum class ERR {
            INVALID_OPERATION,

            INVALID_BINARY_EXPR,
            INVALID_UNARY_EXPR,
            ILL_FORMED_EXPR,

            INVALID_LITERAL,
        };
    } // namespace Constant

    namespace Type
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
    } // namespace Type


    namespace Error
    {
        /// @brief Enqueue an error into the error queue.
        /// @param p_line   The line where the error occured
        /// @param p_column The column where the error occured
        /// @param p_reason The reasoning behind the error.
        /// @param p_phase  Which phased the error occured 
        void syntax(int p_line, int p_column, std::string p_reason) noexcept;
    } // namespace Tool
} // namespace Error
#endif