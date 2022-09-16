#ifndef CENT_SCANNER
#define CENT_SCANNER

#include "Token.h"

#include <string>
#include <vector>


namespace Cent
{
    namespace Tool {
        std::string file2string(std::string p_fp);
    }


    namespace Scanner
    {
        using TokenList = std::vector<Type::TokenData*>;

        TokenList scan(std::string p_source) noexcept;
    }
}

#endif