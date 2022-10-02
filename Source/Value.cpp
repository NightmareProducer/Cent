#include "Value.h"

#include <iostream>
#include <cassert>

namespace Cent
{
    Type::ValueData Int(int p_value)
    {
        return {Constant::ValueType::INT, p_value, 0.0f, ""};
    }

    Type::ValueData Float(float p_value)
    {
        return {Constant::ValueType::FLOAT, 0, p_value, ""};
    }

    Type::ValueData String(std::string p_value)
    {
        return {Constant::ValueType::STRING, 0, 0.0f, p_value};
    }

    Type::ValueData operator+ (const Type::ValueData &p_a, const Type::ValueData &p_b)
    {
        using namespace Constant;

        assert(("A and B must be of the same type", p_a.type == p_b.type));
        
        switch (p_a.type)
        {
        case ValueType::INT:
            return Type::ValueData{p_a.type, p_a.value_int + p_b.value_int};
        case ValueType::FLOAT:
            return Type::ValueData{.type = p_a.type, .value_float = p_a.value_float + p_b.value_float};
        case ValueType::STRING:
            return Type::ValueData{.type = p_a.type, .value_str = p_a.value_str + p_b.value_str};
        }
    
        return Type::ValueData{ValueType::INVALID};
    }

    Type::ValueData operator- (const Type::ValueData &p_a, const Type::ValueData &p_b)
    {
        using namespace Constant;

        assert(("A and B must be of the same type", p_a.type == p_b.type));
        
        switch (p_a.type)
        {
        case ValueType::INT:
            return Type::ValueData{p_a.type, p_a.value_int + p_b.value_int};
        case ValueType::FLOAT:
            return Type::ValueData{.type = p_a.type, .value_float = p_a.value_float - p_b.value_float};
        }
    
        return Type::ValueData{ValueType::INVALID};
    }
}