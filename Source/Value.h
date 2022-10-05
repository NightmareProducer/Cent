#ifndef CENT_VALUE
#define CENT_VALUE

#include "Expressions.h"
#include "Error.h"

#include <memory>
#include <string>
#include <type_traits>
#include <concepts>
#include <variant>


namespace Cent
{
    namespace Constant
    {
        enum ValueType {INT, FLOAT, STRING, BOOL, INVALID};
    }

    namespace Type
    {
        struct ValueData
        {
            Constant::ValueType type = Constant::ValueType::INVALID;
            std::variant<int, float, bool, std::string, EvalErr> content; 
        };
    }

    namespace Tool
    {
        inline bool is_valid_value(const Type::ValueData& p_value)
        {
            return p_value.type != Constant::ValueType::INVALID;
        }

        inline bool is_valid(const Type::ValueData& p_value)
        {
            return is_valid_value(p_value);
        }
    }

    
    constexpr Type::ValueData Value(int p_value);
    constexpr Type::ValueData Value(float p_value);
    constexpr Type::ValueData Value(std::string p_value);
    constexpr Type::ValueData Value(bool p_value);
    Type::ValueData ValueErr(Type::ExprShrd p_expr, Constant::ERR p_errcode);

    Type::ValueData operator* (const Type::ValueData &p_a, const Type::ValueData &p_b);
    Type::ValueData operator+ (const Type::ValueData &p_a, const Type::ValueData &p_b);
    Type::ValueData operator- (const Type::ValueData &p_a, const Type::ValueData &p_b);
    Type::ValueData operator- (const Type::ValueData &p_a);
    Type::ValueData operator! (const Type::ValueData &p_a);
}

#endif