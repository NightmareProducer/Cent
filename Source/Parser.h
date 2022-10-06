#ifndef CENT_PARSER
#define CENT_PARSER

#include "Token.h"
#include "Expressions.h"
#include "Value.h"
#include "Error.h"

#include <concepts>

namespace Cent
{
    namespace Concept
    {
        template<typename T>
        concept ResultWrapper = requires(T x)
        {
            x.data;
            {x.errcode} -> std::convertible_to<Constant::ERR>; 
        };
    }

    namespace Type
    {
        struct ParseResult
        {
            ExprShrd data;
            Constant::ERR errcode;
        };

        struct EvalResult
        {
            ExprShrd from;
            ValueData data;
            Constant::ERR errcode;
        };
    }

    namespace Tool
    {
        using namespace Concept;

        template<ResultWrapper T>
        inline bool is_valid(const T& p_data)
        {
            return p_data.errcode == Constant::ERR::SUCCESS;
        }
    }

    namespace Parser
    {
        Type::ParseResult parse(Type::TokenList p_tokens) noexcept;
        // Type::EvalResult evaluate(const Type::ExprShrd& p_expr) noexcept;
    }
}

#endif