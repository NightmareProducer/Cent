#include "Error.h"

#include <queue>


using namespace Cent;


// { Static variables

/// @brief All error data is stored here. 
static std::queue<Type::ErrorData> s_errors;

//// } End static variables


namespace Cent::Error
{
    void enqueue(int p_line, int p_column, std::string p_reason, Constant::Phase p_phase) noexcept
    {
        s_errors.emplace(p_line, p_column, p_phase, p_reason);
    }
} // namespace Tool


