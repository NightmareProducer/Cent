#ifndef CENT_PARSER
#define CENT_PARSER

#include "Token.h"
#include "Expressions.h"
#include "Value.h"
#include "Error.h"

namespace Cent
{
    namespace Type
    {
        struct ParseResult
        {
            ExprShrd expr;
            Constant::ERR errcode;
        };
    }

    namespace Tool
    {
        inline bool is_valid(const Type::ParseResult& p_data)
        {
            return p_data.errcode == Constant::ERR::SUCCESS;
        }
    }

    namespace Parser
    {
        Type::ParseResult parse(Type::TokenList p_tokens) noexcept;
        Type::ValueData evaluate(const Type::ExprShrd& p_expr) noexcept;
    }
}

#endif