#ifndef CENT_HANDLER
#define CENT_HANDLER

#include "Value.h"
#include "Expressions.h"

#include <concepts>
#include <iostream>

namespace Cent::Concept
{
    template<typename T>
    concept Handleable = std::same_as<T, Type::ValueData> || std::same_as<T, Type::ExprShrd>;
}

namespace Cent
{
    using namespace Concept;

    template<Handleable T, class Fn1, class Fn2> requires std::invocable<Fn1, T> && std::invocable<Fn2, T>
        T handle(const T &p_data, Fn1 p_pass, Fn2 p_fail)
    {
        if (Tool::is_valid(p_data))
            p_pass(p_data);
        else
            p_fail(p_data);

        return p_data; 
    }

    template<Handleable T, class Fn1> requires std::invocable<Fn1, T> 
        T handle(const T &p_data, Fn1 p_pass)
    {
        if (Tool::is_valid(p_data))
            p_pass(p_data);
        
        return p_data; 
    }
}

#endif