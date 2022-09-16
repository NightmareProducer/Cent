#include "Error.h"

#include <queue>


using namespace Cent;


static std::queue<Type::Error> errors;


namespace Cent
{
    namespace Tool
    {
        void enqueue_error(int p_line, int p_column, std::string p_reason, Constant::Phase p_phase) noexcept
        {
            errors.emplace(p_line, p_column, p_phase, p_reason);
        }
    } // namespace Tool
} // namespace Cent


