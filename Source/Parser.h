#ifndef CENT_PARSER
#define CENT_PARSER

#include "Token.h"
#include "Expressions.h"

#include <vector>

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
    }

    namespace Parser
    {
        Type::ExpressionShared parse(Type::TokenList p_tokens) noexcept;
        Type::ValueData* evaluate_expr(Type::ExpressionData *p_expr) noexcept;
    }

    Type::ValueData Value(Constant::ValueType p_type, int p_value);
    Type::ValueData Value(Constant::ValueType p_type, float p_value);
    Type::ValueData Value(Constant::ValueType p_type, std::string p_value);
}

#endif