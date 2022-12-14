#include "Error.hpp"

#include <queue>


using namespace Cent;


// { Static variables

/// @brief All error data is stored here. 
static std::queue<Type::SyntaxError> s_synerr;

//// } End static variables


namespace Cent::Error
{
    void syntax(int p_line, int p_column, std::string p_reason) noexcept
    {
        s_synerr.emplace(p_line, p_column, p_reason);
    }
} // namespace Tool


