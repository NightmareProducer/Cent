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
        Type::ExprShrd parse(Type::TokenList p_tokens) noexcept;
        Type::ValueData evaluate_expr(const Type::ExprShrd& p_expr) noexcept;
    }
}

#endif