#pragma once

#include "../Application.h"
#include <stdio.h>
#include <mutex>

namespace IE
{
	namespace IELogger
	{
		enum class IsoEngine_API IELogger_Priority
		{
			TRACE		= 0, // Minor Details
			DEBUG		= 1, // Debugger Info
			INFO		= 2, // Recording significant events/updates
			WARN		= 3, // Warnings related to exceptions and/or bugs		
			CRITICAL	= 4, // Error -- Failure to perform a certain task
			FATAL		= 5 // Error -- Resulting in crash or termination of program
		};

		class IsoEngine_API IsoLogger
		{
		private:
			static IELogger_Priority priority_level; // must be initialized outside class body
			static std::mutex logger_mutex;

			template<typename... Args>
			static void IsoLog(IE::IELogger::IELogger_Priority msg_priority, const char* msg_priority_str, const char* msg, Args... args)
			{
				if (priority_level <= msg_priority)
				{
					std::scoped_lock lock(logger_mutex);
					printf(msg_priority_str, msg, args...);
					printf("\n");
				}
			}

		public:
			//IsoLogger();
			//virtual ~IsoLogger();

			static void SetPriority(IELogger_Priority priority) { priority_level = priority; }

			template<typename... Args>
			static void Trace(const char* message, Args... args)
			{
				IsoLog(IE::IELogger::IELogger_Priority::TRACE, "[TRACE]:\t", message, args...);
			}

			template<typename... Args>
			static void Debug(const char* message, Args... args)
			{
				IsoLog(IE::IELogger::IELogger_Priority::DEBUG, "[DEBUG]:\t", message, args...);
			}

			template<typename... Args>
			static void Info(const char* message, Args... args)
			{
				IsoLog(IE::IELogger::IELogger_Priority::INFO, "[INFO]:\t", message, args...);
			}

			template<typename... Args>
			static void Warn(const char* message, Args... args)
			{
				IsoLog(IE::IELogger::IELogger_Priority::WARN, "[WARN]:\t", message, args...);
			}

			template<typename... Args>
			static void Critical(const char* message, Args... args)
			{
				IsoLog(IE::IELogger::IELogger_Priority::CRITICAL, "[CRITICAL]:\t", message, args...);
			}

			template<typename... Args>
			static void Fatal(const char* message, Args... args)
			{
				IsoLog(IE::IELogger::IELogger_Priority::FATAL, "[FATAL]:\t", message, args...);
			}
		};

		IE::IELogger::IELogger_Priority IE::IELogger::IsoLogger::priority_level = IE::IELogger::IELogger_Priority::INFO; // Default Priority Level
		std::mutex IE::IELogger::IsoLogger::logger_mutex;
		//IsoLogger* IsoLogger_Init();
	}
}
