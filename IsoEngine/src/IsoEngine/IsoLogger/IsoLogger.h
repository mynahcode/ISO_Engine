#pragma once

#include <string>

#include "../Application.h"

namespace IE
{
	namespace IELogger
	{
		enum class IELogger_Priority
		{
			TRACE		= 0,						// Trace level for function calls, tracking processes of program.
			DEBUG		= 1,						// Info Level Debugging.
			INFO		= 2,						// Recording significant events, updates, etc.
			WARN		= 3,						// Warnings related to exceptions and/or bugs, or edge cases being met.		
			CRITICAL	= 4,						// Error -- Failure to perform a certain task
			FATAL		= 5							// Error -- Resulting in crash or termination of program
		};

		/* Re-factored to be a Singleton design pattern */
		class IsoEngine_API IsoLogger
		{

		private:

			/* Member Variable Initializations */
			IELogger_Priority priority_level = IELogger_Priority::INFO;		// Default Priority Level
			std::mutex logger_mutex;
			const char* filepath = 0;
			FILE* file;

			IsoLogger() {}
			IsoLogger(const IsoLogger&) = delete;							// Copy-Constructor
			IsoLogger& operator = (const IsoLogger&) = delete;				// Assignment Constructor
			~IsoLogger() { free_File(); }

			/* Getter Function to get instance of IsoLogger */
			static IsoLogger& get_InstanceIsoLogger()
			{
				static IsoLogger iso_logger;								// Global variable accessed inside of the function -- initialized only once
				return iso_logger;
			}

			template<typename... Args>
			void IsoLog(IELogger::IELogger_Priority msg_priority, const char* msg_priority_str, const char* msg, Args... args)
			{
				if (priority_level <= msg_priority)
				{

					std::scoped_lock lock(logger_mutex);

					errno_t err;
					struct tm curr_time;
					__time64_t long_time;
					char buffer[26];
					char am_pm[] = "AM";

					_time64(&long_time);

					err = _localtime64_s(&curr_time, &long_time);

					if (err)
					{
						printf("Invalid argument to _localtime64_s.");
					}

					if (curr_time.tm_hour > 12)								// Set up extension
						strcpy_s(am_pm, sizeof(am_pm), "PM");
					if (curr_time.tm_hour >= 12)							// Convert from 24-hour time
						curr_time.tm_hour -= 12;							// to 12 hour time
					if (curr_time.tm_hour == 0)								// Set hour to 12 if curr_time.tm_hour - 12 == 0 (midnight)
						curr_time.tm_hour = 12;

					// Convert to an ASCII representation
					err = asctime_s(buffer, 26, &curr_time);
					if (err)
					{
						printf("Invalid argument to asctime_s");
					}

					printf("[%.19s %s]", buffer, am_pm);
					printf(msg_priority_str);
					printf(msg, args...);
					printf("\n");


					// Enable File-Logging Check
					if (file)
					{
						fprintf(file, "%.19s %s", buffer, am_pm);
						fprintf(file, msg_priority_str);
						fprintf(file, msg, args...);
						fprintf(file, "\n");
					}
				}
			}

			template<typename... Args>
			void IsoLog(int line_num, const char* src_file, IELogger::IELogger_Priority msg_priority, const char* msg_priority_str, const char* msg, Args... args)
			{
				if (priority_level <= msg_priority)
				{

					std::scoped_lock lock(logger_mutex);

					errno_t err;
					struct tm curr_time;
					__time64_t long_time;
					char buffer[26];
					char am_pm[] = "AM";

					_time64(&long_time);

					err = _localtime64_s(&curr_time, &long_time);

					if (err)
					{
						printf("Invalid argument to _localtime64_s.");
					}
					
					if (curr_time.tm_hour > 12)								// Set up extension
						strcpy_s(am_pm, sizeof(am_pm), "PM");
					if (curr_time.tm_hour >= 12)							// Convert from 24-hour time
						curr_time.tm_hour -= 12;							// to 12 hour time
					if (curr_time.tm_hour == 0)								// Set hour to 12 if curr_time.tm_hour - 12 == 0 (midnight)
						curr_time.tm_hour = 12;

					// Convert to an ASCII representation
					err = asctime_s(buffer, 26, &curr_time);
					if (err)
					{
						printf("Invalid argument to asctime_s");
					}

					printf("[%.19s %s]", buffer, am_pm);
					printf(msg_priority_str);
					printf(msg, args...);
					printf(" On Line %d in %s", line_num, src_file);
					printf("\n");


					// Enable File-Logging Check
					if (file)
					{
						fprintf(file, "%.19s %s", buffer, am_pm);
						fprintf(file, msg_priority_str);
						fprintf(file, msg, args...);
						fprintf(file, " On Line %d in %s", line_num, src_file);
						fprintf(file, "\n");
					}
				}
			}

			void enable_IsoFileOut()
			{
				if (file != 0)
				{
					fclose(file);
				}

				file = fopen(filepath, "a");
				if (file == 0)
				{
					printf("<<IsoLogger>> Failed to open file at %s", filepath);
				}
			}

			void free_File()
			{
				fclose(file);
				file = 0;
			}

		public:

			static void SetPriority(IELogger_Priority priority) 
			{ 
				get_InstanceIsoLogger().priority_level = priority; 
			}

			static void EnableIsoFileOut()
			{
				IsoLogger& isologger_instance = get_InstanceIsoLogger();
				isologger_instance.filepath = "log.txt";
				isologger_instance.enable_IsoFileOut();
			}

			static void EnableIsoFileOut(const char* new_filepath)
			{
				IsoLogger& isologger_instance = get_InstanceIsoLogger();
				isologger_instance.filepath = new_filepath;
				isologger_instance.enable_IsoFileOut();
			}

			template<typename... Args>
			static void Trace(int line_num, const char* src_file, const char* message, Args... args)
			{
				get_InstanceIsoLogger().IsoLog(line_num, src_file, IE::IELogger::IELogger_Priority::TRACE, "[TRACE]:\t", message, args...);
			}

			template<typename... Args>
			static void Debug(int line_num, const char* src_file, const char* message, Args... args)
			{
				get_InstanceIsoLogger().IsoLog(line_num, src_file, IE::IELogger::IELogger_Priority::DEBUG, "[DEBUG]:\t", message, args...);
			}

			template<typename... Args>
			static void Info(int line_num, const char* src_file, const char* message, Args... args)
			{
				get_InstanceIsoLogger().IsoLog(line_num, src_file, IE::IELogger::IELogger_Priority::INFO, "[INFO]:\t", message, args...);
			}

			template<typename... Args>
			static void Warn(int line_num, const char* src_file, const char* message, Args... args)
			{
				get_InstanceIsoLogger().IsoLog(line_num, src_file, IE::IELogger::IELogger_Priority::WARN, "[WARN]:\t", message, args...);
			}

			template<typename... Args>
			static void Critical(int line_num, const char* src_file, const char* message, Args... args)
			{
				get_InstanceIsoLogger().IsoLog(line_num, src_file, IE::IELogger::IELogger_Priority::CRITICAL, "[CRITICAL]:\t", message, args...);
			}

			template<typename... Args>
			static void Fatal(int line_num, const char* src_file, const char* message, Args... args)
			{
				get_InstanceIsoLogger().IsoLog(line_num, src_file, IE::IELogger::IELogger_Priority::FATAL, "[FATAL]:\t", message, args...);
			}

		};
	}
}

/* IsoLogger Macro Definitions -- Overloads IsoLog function call parameters within each respective severity-level function call*/
#define ISOLOGGER_TRACE(Message,...) (::IE::IELogger::IsoLogger::Trace(__LINE__, __FILE__, Message, __VA_ARGS__))
#define ISOLOGGER_DEBUG(Message,...) (::IE::IELogger::IsoLogger::Debug(__LINE__, __FILE__, Message, __VA_ARGS__))
#define ISOLOGGER_INFO(Message,...) (::IE::IELogger::IsoLogger::Info(__LINE__, __FILE__, Message, __VA_ARGS__))
#define ISOLOGGER_WARN(Message,...) (::IE::IELogger::IsoLogger::Warn(__LINE__, __FILE__, Message, __VA_ARGS__))
#define ISOLOGGER_CRITICAL(Message,...) (::IE::IELogger::IsoLogger::Critical(__LINE__, __FILE__, Message, __VA_ARGS__))
#define ISOLOGGER_FATAL(Message,...) (::IE::IELogger::IsoLogger::Fatal(__LINE__, __FILE__, Message, __VA_ARGS__))
