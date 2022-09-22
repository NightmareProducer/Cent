#ifndef CENT_PARSER
#define CENT_PARSER

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