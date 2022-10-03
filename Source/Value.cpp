#include "Value.h"

#include <iostream>
#include <cassert>

namespace Cent
{
    Type::ValueData Int(int p_value)
    {
        return {.type = Constant::ValueType::INT, .value_int = p_value};
    }

    Type::ValueData Float(float p_value)
    {
        return {.type = Constant::ValueType::FLOAT, .value_float = p_value};
    }

    Type::ValueData String(std::string p_value)
    {
        return {.type = Constant::ValueType::STRING, .value_str = p_value};
    }

    Type::ValueData Bool(bool p_value)
    {
        return {.type = Constant::ValueType::BOOL, .value_int = p_value};
    }

    Type::ValueData operator+ (const Type::ValueData &p_a, const Type::ValueData &p_b)
    {
        using namespace Constant;

        assert(("A and B must be of the same type", p_a.type == p_b.type));
        
        switch (p_a.type)
        {
        case ValueType::INT:
            return Int(p_a.value_int + p_b.value_int);
        case ValueType::FLOAT:
            return Float(p_a.value_float + p_b.value_float);
        case ValueType::STRING:
            return String(p_a.value_str + p_b.value_str);
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
            return Int(p_a.value_int + p_b.value_int);
        case ValueType::FLOAT:
            return Float(p_a.value_float + p_b.value_float);
        }
    
        return Type::ValueData{ValueType::INVALID};
    }
}