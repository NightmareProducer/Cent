#include "Value.hpp"
#include "Error.hpp"
#include "Expressions.hpp"

#include <iostream>


namespace Cent
{
    Type::ValueData Value(int p_value)
    {
        return {Constant::ValueType::INT, p_value};
    }

    Type::ValueData Value(float p_value)
    {
        return {Constant::ValueType::FLOAT, p_value};
    }

    Type::ValueData Value(std::string p_value)
    {
        return {Constant::ValueType::STRING, p_value};
    }

    Type::ValueData Value(bool p_value)
    {
        return {Constant::ValueType::BOOL, p_value};
    }

    Type::ValueData InvalidValue()
    {
        return {Constant::ValueType::INVALID};
    }


    Type::ValueData operator* (const Type::ValueData &p_a, const Type::ValueData &p_b)
    {
        using namespace Constant;

        if(p_a.type != p_b.type)
            return InvalidValue();
        
        switch (p_a.type)
        {
        case ValueType::INT:
            return Value(std::get<int>(p_a.content) * std::get<int>(p_b.content));
        case ValueType::FLOAT:
            return Value(std::get<float>(p_a.content) * std::get<float>(p_b.content));
        }
    
        return InvalidValue();
    }

    Type::ValueData operator+ (const Type::ValueData &p_a, const Type::ValueData &p_b)
    {
        using namespace Constant;

        if(p_a.type != p_b.type)
            return InvalidValue();
        
        switch (p_a.type)
        {
        case ValueType::INT:
            return Value(std::get<int>(p_a.content) + std::get<int>(p_b.content));
        case ValueType::FLOAT:
            return Value(std::get<float>(p_a.content) + std::get<float>(p_b.content));
        case ValueType::STRING:
            return Value(std::get<std::string>(p_a.content) + std::get<std::string>(p_b.content));
        }
    
        return InvalidValue();
    }

    Type::ValueData operator- (const Type::ValueData &p_a, const Type::ValueData &p_b)
    {
        using namespace Constant;

        switch (p_a.type)
        {
        case ValueType::INT:
            return Value(std::get<int>(p_a.content) - std::get<int>(p_b.content));
        case ValueType::FLOAT:
            return Value(std::get<float>(p_a.content) - std::get<float>(p_b.content));
        }
    
        return InvalidValue();
    }

    Type::ValueData operator- (const Type::ValueData &p_a)
    {
        using namespace Constant;

        switch (p_a.type)
        {
        case ValueType::INT:
            return Value(-std::get<int>(p_a.content));
        case ValueType::FLOAT:
            return Value(-std::get<float>(p_a.content));
        }

        return InvalidValue();
    }


    Type::ValueData operator! (const Type::ValueData &p_a)
    {
        using namespace Constant;

        switch (p_a.type)
        {
        case ValueType::BOOL:
            return Value(!std::get<bool>(p_a.content));
        }

        return InvalidValue();
    }
}