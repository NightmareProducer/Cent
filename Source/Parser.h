#ifndef CENT_PARSER
#define PARSER

#include <Token.h>

#include <vector>

namespace Cent
{
    namespace Parser
    {
        void parse(Type::TokenList &p_tokens) noexcept;
    }
}

#endif