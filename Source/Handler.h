#ifndef CENT_HANDLER
#define CENT_HANDLER

#include "Value.h"
#include "Expressions.h"
#include "Parser.h"


#include <concepts>
#include <utility>


namespace Cent::Concept
{
    template<typename T>
    concept Handleable = requires(T x) 
    {
        Tool::is_valid(x); // Has a valid overload
        x.data;            // Has .data field
    };
    
    template<typename Fn, typename T>
    concept Handler = std::invocable<Fn, decltype(std::declval<T>().data)>;
}

namespace Cent
{
    using namespace Concept;

    template<Handleable T, Handler<T> Fn1, Handler<T> Fn2> 
        T handle(T &&p_data, Fn1 p_pass, Fn2 p_fail)
    {
        if (Tool::is_valid(p_data))
            p_pass(p_data.data);
        else
            p_fail(p_data);

        return p_data; 
    }

    template<Handleable T, Handler<T> Fn1>
        T handle(T &&p_data, Fn1 p_pass)
    {
        if (Tool::is_valid(p_data))
            p_pass(p_data.data);
        
        return p_data; 
    }
}

#endif