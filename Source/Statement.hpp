#ifndef CENT_STMT
#define CENT_STMT

#include "Expressions.hpp"

#include <concepts>

namespace Cent
{
    namespace Type
    {
        struct Stmt
        {
            ExprShrd expr;
        };
    } // namespace Type


} // namespace Cent

#endif