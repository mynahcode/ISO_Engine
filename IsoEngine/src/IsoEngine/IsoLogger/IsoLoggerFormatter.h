
#pragma once
#include "iepch.h"

namespace IE
{
    class IsoLoggerFormatter
    {
    public:

        // Base IsoLoggerFormatter function to be overriden by Variadic Function
        static inline std::ostream& IsoLoggerRecursive(std::ostream& log_out, const char* log_msg)
        {
            return log_out << log_msg;
        }

        // "Recursive" variadic function -- hacky solution to iterate through arbitrary amount of __VA_ARGS__ passed to logging function
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