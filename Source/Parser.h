#ifndef CENT_PARSER
#define CENT_PARSER

#include "Token.h"
#include "Expressions.h"

#include <vector>
#include <memory>

namespace Cent
{
    namespace Constant
    {
        enum ValueType {STRING, FLOAT, INT};
    }

    namespace Type
    {
        struct ValueData
        {
            Constant::ValueType type;
            int value_int;
            float value_float;
            std::string value_str;
        };

        using ValuePtr = std::unique_ptr<ValueData>;
        using ValueShrd = std::unique_ptr<ValueData>;
    }

    namespace Parser
    {
        Type::ExpressionShrd parse(Type::TokenList p_tokens) noexcept;
        Type::ValuePtr evaluate_expr(Type::ExpressionData *p_expr) noexcept;
    }

    Type::ValuePtr Value(Constant::ValueType p_type, int p_value);
    Type::ValuePtr Value(Constant::ValueType p_type, float p_value);
    Type::ValuePtr Value(Constant::ValueType p_type, std::string p_value);
}

#endif