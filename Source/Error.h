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
        struct ErrorData 
        {
            int line;
            int column;
            Constant::Phase phase;
            std::string reason;
        };
    } // namespace Type


    namespace Error
    {
        /// @brief Enqueue an error into the error queue.
        /// @param p_line   The line where the error occured
        /// @param p_column The column where the error occured
        /// @param p_reason The reasoning behind the error.
        /// @param p_phase  Which phased the error occured 
        void enqueue(int p_line, int p_column, std::string p_reason, Constant::Phase p_phase) noexcept;
    } // namespace Tool
} // namespace Error
#endif