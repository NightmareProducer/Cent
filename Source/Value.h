#ifndef CENT_VALUE
#define CENT_VALUE

#include "Expressions.h"

#include <memory>
#include <string>
#include <type_traits>
#include <concepts>
#include <variant>

namespace Cent
{
    namespace Constant
    {
        enum ValueType {INVALID = -1, STRING, FLOAT, INT, BOOL};
    }

    namespace Type
    {
        struct ValueData
        {
            Constant::ValueType type = Constant::ValueType::INVALID;
            std::variant<int, float, std::string> content; 
            /// @brief This field is only assigned when the ValueData is returned  
            ///        from evaluate_expression and the .type field is set to Invalid.
            ExprShrd eval_expr_err = nullptr; 
        };
    }

    namespace Tool
    {
        inline bool is_invalid_value(const Type::ValueData& p_value)
        {
            return p_value.type == Constant::ValueType::INVALID;
        }
    }

    
    Type::ValueData Value(int p_value);
    Type::ValueData Value(float p_value);
    Type::ValueData Value(std::string p_value);

    Type::ValueData operator+ (const Type::ValueData &p_a, const Type::ValueData &p_b);
    Type::ValueData operator- (const Type::ValueData &p_a, const Type::ValueData &p_b);
}

#endif