#ifndef CENT_SCANNER
#define CENT_SCANNER

#include "Token.hpp"

#include <string>
#include <vector>


namespace Cent
{
    namespace Tool {
        std::string file2string(std::string p_fp);
    }


    namespace Scanner
    {
        Type::TokenList scan(std::string p_source) noexcept;
    }
}

#endif