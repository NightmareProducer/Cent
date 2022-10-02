#ifndef CENT_VALUE
#define CENT_VALUE

#include <memory>
#include <string>
#include <type_traits>

namespace Cent
{
    namespace Constant
    {
        enum ValueType {INVALID = -1, STRING, FLOAT, INT};
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

    namespace Tool
    {
        inline bool is_invalid_value(const Type::ValueData& p_value)
        {
            return p_value.type == Constant::ValueType::INVALID;
        }
    }

    Type::ValueData Int(int p_value);
    Type::ValueData Float(float p_value);
    Type::ValueData String(std::string p_value);

    Type::ValueData operator+ (const Type::ValueData &p_a, const Type::ValueData &p_b);
    Type::ValueData operator- (const Type::ValueData &p_a, const Type::ValueData &p_b);
}

#endif