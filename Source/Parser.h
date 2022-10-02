#ifndef CENT_PARSER
#define CENT_PARSER

#include "Token.h"
#include "Expressions.h"
#include "Value.h"

#include <vector>
#include <memory>

namespace Cent
{
    namespace Parser
    {
        Type::ExpressionShrd parse(Type::TokenList p_tokens) noexcept;
    }
}

#endif