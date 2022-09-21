#ifndef CENT_PARSER
#define PARSER

#include "Token.h"
#include "Expressions.h"

#include <vector>

namespace Cent
{
    namespace Parser
    {
        Type::ExpressionData* parse(Type::TokenList &p_tokens) noexcept;
    }
}

#endif