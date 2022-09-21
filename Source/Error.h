#ifndef CENT_ERROR
#define CENT_ERROR

#include <string>

namespace Cent
{
    namespace Constant
    {
        enum class Phase { SCANNING, PARSING };
    } // namespace Constant

    namespace Type
    {
        struct Error 
        {
            int line;
            int column;
            Constant::Phase phase;
            std::string reason;
        };
    } // namespace Type


    namespace Error
    {
        void enqueue(int p_line, int p_column, std::string p_reason, Constant::Phase p_phase) noexcept;
    } // namespace Tool
} // namespace Error
#endif