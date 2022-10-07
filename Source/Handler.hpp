#ifndef CENT_HANDLER
#define CENT_HANDLER

#include "Value.hpp"
#include "Expressions.hpp"
#include "Parser.hpp"
#include "Token.hpp"

#include <concepts>
#include <utility>
#include <iostream>
#include <variant>
#include <type_traits>


namespace Cent::Concept
{
    template<typename T>
    concept isHandleable = requires(T x) { Tool::is_valid(x); };
    
    template<typename Fn, typename T>
    concept isHandler = std::invocable<Fn, T>;

    template<typename T>
    concept HasDataField = requires(T x) { x.data; };
}

namespace Cent
{
    using namespace Concept;

    template<isHandleable T, isHandler<T> Fn1, isHandler<T> Fn2>
        T handle(T &&p_data, Fn1 p_pass, Fn2 p_fail)
    {
        if (Tool::is_valid(p_data))
            p_pass(p_data);
        else
            p_fail(p_data);
        
        return p_data; 
    }

    template<isHandleable T, isHandler<T> Fn>
        T handle(T &&p_data, Fn p_pass)
    {
        if (Tool::is_valid(p_data))
            p_pass(p_data);
        
        return p_data; 
    }

    template<isHandleable T, isHandler<T> Fn1>
        T handle_fail(T &&p_data, Fn1 p_fail)
    {
        if (!Tool::is_valid(p_data))
            p_fail(p_data);
        
        return p_data;
    }
}


namespace Cent::Handler
{
    using namespace Concept;

    const auto print_fail = []<HasDataField T> (T &&p_data) -> void
    {
        using namespace Type;
        using namespace Constant;

        if constexpr(isExprParseRes<T>)
        {
            auto& tkn = std::get<TokenShrd>(p_data.data->content);

            std::cout << tkn->lexeme << std::endl;
        }
    };
}

#endif