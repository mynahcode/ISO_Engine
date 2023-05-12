#pragma once
#include "iepch.h"

namespace IE
{
    class IsoLoggerFormatter
    {
    public:

        static inline std::ostream& IsoLoggerRecursive(std::ostream& log_out, const char* log_msg)
        {
            return log_out << log_msg;
        }

        // "Recursive" variadic function
        template<typename First, typename... Args>
        static inline std::ostream& IsoLoggerRecursive(std::ostream& log_out, const char* log_msg, First&& first, Args&&... args)
        {
            while (*log_msg)
            {
                if (*log_msg == '%')
                {
                    return IsoLoggerRecursive(log_out << std::forward<First>(first), ++log_msg, std::forward<Args>(args)...);
                }
                log_out << *log_msg++;
            }

            return log_out;
        }
    };
}