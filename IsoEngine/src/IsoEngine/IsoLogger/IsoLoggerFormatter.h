#pragma once
#include "iepch.h"

namespace IE
{
    class IsoLoggerFormatter
    {
    public:
        // Log_Recursive wrapper that creates the ostringstream
        template<typename... Args>
        static void IsoLoggerWrapper(int line, const char* file, const char* log_msg, const Args&... args)
        {
            std::ostringstream log_out;
            log_out << log_msg;
            IsoLoggerRecursive(line, file, log_out, args...);
        }

        // "Recursive" variadic function
        template<typename T, typename... Args>
        static void IsoLoggerRecursive(int line, const char* file, std::ostringstream& log_out,
            T value, const Args&... args)
        {
            log_out << value;
            IsoLoggerRecursive(line, file, log_out, args...);
        }

        // Terminator
        static void IsoLoggerRecursive(int line, const char* file, std::ostringstream& log_out)
        {
            std::cout << log_out.str() << " - " << file << " (On line " << line << ")" << std::endl;
        }
    };
}