#pragma once

#include <stdio.h>
#include <mutex>
#include <ctime>

#include "../Application.h"

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
			static IELogger::IELogger_Priority priority_level; // must be initialized outside class body
			static std::mutex logger_mutex;
			static const char* filepath;
			static FILE* file;

			template<typename... Args>
			static void IsoLog(IELogger::IELogger_Priority msg_priority, const char* msg_priority_str, const char* msg, Args... args)
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
					
					if (curr_time.tm_hour > 12)					// Set up extension
						strcpy_s(am_pm, sizeof(am_pm), "PM");
					if (curr_time.tm_hour >= 12)				// Convert from 24-hour time
						curr_time.tm_hour -= 12;				// to 12 hour time
					if (curr_time.tm_hour == 0)					// Set hour to 12 if curr_time.tm_hour - 12 == 0 (midnight)
						curr_time.tm_hour = 12;

					// Convert to an ASCII representation
					err = asctime_s(buffer, 26, &curr_time);
					if (err)
					{
						printf("Invalid argument to asctime_s");
					}

					printf("[%.19s %s\n]", buffer, am_pm);
					printf(msg_priority_str);
					printf(msg, args...);
					printf("\n");


					// Enable File-Logging Check
					if (file)
					{
						fprintf(file, "%.19s %s\n", buffer, am_pm);
						fprintf(file,msg_priority_str);
						fprintf(file,msg, args...);
						fprintf(file, "\n");
					}
				}
			}

			static void enable_IsoFileOut()
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

			static void free_File()
			{
				fclose(file);
				file = 0;
			}

		public:
			//IsoLogger();
			//virtual ~IsoLogger();

			static void SetPriority(IELogger_Priority priority) { priority_level = priority; }

			static void EnableIsoFileOut()
			{
				filepath = "log.txt";
				enable_IsoFileOut();
			}

			static void EnableIsoFileOut(const char* new_filepath)
			{
				filepath = new_filepath;
				enable_IsoFileOut();
			}

			static void CloseFileOut()
			{
				free_File();
			}

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

		/* Member Variable Initializations */
		IELogger_Priority IsoLogger::priority_level = IELogger_Priority::INFO; // Default Priority Level
		std::mutex IsoLogger::logger_mutex;
		const char* IsoLogger::filepath = 0;
		FILE* IsoLogger::file;

	}
}
