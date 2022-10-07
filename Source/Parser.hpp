#ifndef CENT_PARSER
#define CENT_PARSER

#include "Token.hpp"
#include "Expressions.hpp"
#include "Value.hpp"
#include "Error.hpp"

#include <concepts>
#include <type_traits>

namespace Cent
{
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

    namespace Concept
    {
        using namespace Type;
        using namespace Constant;

        template<typename T>
        concept ResultWrapper = requires(T x)
        {
            x.data;
            {x.errcode} -> std::convertible_to<ERR>; 
        };

        template<typename T>
        concept isParseResult = std::same_as<std::decay_t<T>, ParseResult>;
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
        Type::EvalResult evaluate(const Type::ExprShrd& p_expr) noexcept;
    }
}

#endif