#ifndef CENT_HANDLER
#define CENT_HANDLER

#include "Value.h"
#include "Expressions.h"
#include "Parser.h"

#include <concepts>


namespace Cent::Concept
{
    template<typename T>
    concept Handleable = std::same_as<T, Type::ValueData> || 
                         std::same_as<T, Type::ExprShrd>  ||
                         std::same_as<T, Type::ParseResult> ||
                         std::same_as<T, Type::ParseResult&&>;
    
    template<typename Fn, typename T>
    concept Handler = std::invocable<Fn, T>;
}

namespace Cent
{
    using namespace Concept;

    template<Handleable T, Handler<T> Fn> inline 
    void _CallStub(Fn p_fn, T&& p_data) requires std::invocable<Fn, T>
    {
        using namespace Type;

        if constexpr (std::same_as<T, ParseResult>)
            p_fn(p_data);
        else 
            p_fn(p_data);
    }

    template<Handleable T, Handler<T> Fn1, Handler<T> Fn2> 
        T handle(T &&p_data, Fn1 p_pass, Fn2 p_fail)
    {
        if (Tool::is_valid(p_data))
            _CallStub(p_pass, std::move(p_data));
        else
            _CallStub(p_fail, std::move(p_data));

        return p_data; 
    }

    template<Handleable T, Handler<T> Fn1>
        T handle(T &&p_data, Fn1 p_pass)
    {
        if (Tool::is_valid(p_data))
            _CallStub(p_pass, std::move(p_data));
        
        return p_data; 
    }
}

#endif