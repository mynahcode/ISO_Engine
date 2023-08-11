#pragma once

#include <string>
#include <include/fmt/core.h>
#include <include/fmt/format.h>
#include <include/fmt/args.h>
#include <include/fmt/format-inl.h>
#include <include/fmt/ranges.h>
#include <include/fmt/chrono.h>
#include <include/fmt/color.h>
#include <include/fmt/os.h>
#include <include/fmt/ostream.h>

using namespace std::literals::chrono_literals;

// TODO: Refactor IsoLogger to integrate fmt library functionality/printing for logger console and file outputs.
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
			FATAL		= 5							// Fatal -- Resulting in crash or termination of program
		};

		class IsoLogger
		{

		private:

			/* Member Variable Initializations */
			static IELogger_Priority priority_level;
			std::mutex logger_mutex;
			bool IsoLoggerOut = false;

			IsoLogger() = default;
			IsoLogger(const IsoLogger&) = delete;							// Copy-Constructor
			IsoLogger& operator = (const IsoLogger&) = delete;				// Assignment Constructor
			~IsoLogger() { free_File(); }

			/* Getter Function to get instance of IsoLogger */
			static IsoLogger& get_InstanceIsoLogger()
			{
				static IsoLogger iso_logger;								// Global variable accessed inside of the function -- initialized only once
				return iso_logger;
			}

			template<typename... T>
			void IsoLog(int line_num, fmt::string_view src_file, IELogger_Priority msg_priority, const char* priority, fmt::string_view format, fmt::format_args args)
			{
				if (priority_level <= msg_priority)
				{
					if (!IsoLoggerOut)
					{
						std::scoped_lock lock(logger_mutex);
						std::time_t tp = std::time(nullptr);
						auto const curr_time = fmt::localtime(tp);

						fmt::print("[{:%F %r}]  ", curr_time);

						switch (msg_priority)
						{
						case IELogger_Priority::TRACE:
							fmt::print(bg(fmt::color::rebecca_purple), priority);
							break;

						case IELogger_Priority::DEBUG:
							fmt::print(bg(fmt::color::royal_blue), priority);
							break;
						case IELogger_Priority::INFO:
							fmt::print(bg(fmt::color::forest_green), priority);
							break;

						case IELogger_Priority::WARN:
							fmt::print(bg(fmt::color::dark_golden_rod), priority);
							break;

						case IELogger_Priority::CRITICAL:
							fmt::print(bg(fmt::color::crimson), priority);
							break;

						case IELogger_Priority::FATAL:
							fmt::print(bg(fmt::color::dark_red), priority);
							break;
						}

						fmt::print(" in {} ({:d})  ", src_file, line_num);
						fmt::vprint(format, args);
					}
					// TODO: Fix file logging and refactor for supporting unique log files for the engine, the editor, and the application
					else
					{
						std::scoped_lock lock(logger_mutex);
						//fmt::ostream out = fmt::output_file("./TheGame/logs/log.txt", fmt::file::WRONLY | fmt::file::CREATE | fmt::file::APPEND);
						FILE* file = std::fopen("log.txt", "w");
						std::time_t tp = std::time(nullptr);
						auto const curr_time = fmt::localtime(tp);
						fmt::print(file, "[{:%F %r}]  ", curr_time);
						fmt::print(file, " {} in {} ({:d})  ", priority, src_file, line_num);
						fmt::vprint(file, format, args);
						std::fclose(file);
					}
				}
			}

			void free_File()
			{
				//fclose(file);
				//file = 0;
			}

		public:
			inline static void SetPriority(IELogger_Priority priority) 
			{ 
				get_InstanceIsoLogger().priority_level = priority; 
			}

			static void EnableIsoFileOut(const char* new_filepath)
			{
				IsoLogger& isologger_instance = get_InstanceIsoLogger();
				//isologger_instance.filepath = new_filepath;
				//isologger_instance.enable_IsoFileOut();
			}

			template<typename... T>
			static void Trace(int line_num, fmt::format_string<T...> src_file, fmt::format_string<T...> format, T&&... args)
			{
				get_InstanceIsoLogger().IsoLog(line_num, src_file, IELogger_Priority::TRACE, "[TRACE]:", format, fmt::make_format_args(args...));
			}

			template<typename... T>
			static void Debug(int line_num, fmt::format_string<T...> src_file, fmt::format_string<T...> format, T&&... args)
			{
				get_InstanceIsoLogger().IsoLog(line_num, src_file, IELogger_Priority::DEBUG, "[DEBUG]:", format, fmt::make_format_args(args...));
			}

			template<typename... T>
			static void Info(int line_num, fmt::format_string<T...> src_file, fmt::format_string<T...> format, T&&... args)
			{
				get_InstanceIsoLogger().IsoLog(line_num, src_file, IELogger_Priority::INFO, "[INFO]:", format, fmt::make_format_args(args...));
			}

			template<typename... T>
			static void Warn(int line_num, fmt::format_string<T...> src_file, fmt::format_string<T...> format, T&&... args)
			{
				get_InstanceIsoLogger().IsoLog(line_num, src_file, IELogger_Priority::WARN, "[WARN]:", format, fmt::make_format_args(args...));
			}

			template<typename... T>
			static void Critical(int line_num, fmt::format_string<T...> src_file, fmt::format_string<T...> format, T&&... args)
			{
				get_InstanceIsoLogger().IsoLog(line_num, src_file, IELogger_Priority::CRITICAL, "[CRITICAL]:", format, fmt::make_format_args(args...));
			}

			template<typename... T>
			static void Fatal(int line_num, fmt::format_string<T...> src_file, fmt::format_string<T...> format, T&&... args)
			{
				get_InstanceIsoLogger().IsoLog(line_num, src_file, IELogger_Priority::FATAL, "[FATAL]:", format, fmt::make_format_args(args...));
			}

		};
	}
}

/* IsoLogger Macro Definitions -- Overloads IsoLog function call parameters within each respective severity-level function call*/
#define ISOLOGGER_TRACE(Message,...) { IE::IELogger::IsoLogger::SetPriority(IE::IELogger::IELogger_Priority::TRACE); (::IE::IELogger::IsoLogger::Trace(__LINE__, __FILE__, Message, __VA_ARGS__)); }
#define ISOLOGGER_DEBUG(Message,...) { IE::IELogger::IsoLogger::SetPriority(IE::IELogger::IELogger_Priority::DEBUG); (::IE::IELogger::IsoLogger::Debug(__LINE__, __FILE__, Message, __VA_ARGS__)); }
#define ISOLOGGER_INFO(Message,...) { IE::IELogger::IsoLogger::SetPriority(IE::IELogger::IELogger_Priority::INFO); (::IE::IELogger::IsoLogger::Info(__LINE__, __FILE__, Message, __VA_ARGS__)); }
#define ISOLOGGER_WARN(Message,...) { IE::IELogger::IsoLogger::SetPriority(IE::IELogger::IELogger_Priority::WARN); (::IE::IELogger::IsoLogger::Warn(__LINE__, __FILE__, Message, __VA_ARGS__)); }
#define ISOLOGGER_CRITICAL(Message,...) { IE::IELogger::IsoLogger::SetPriority(IE::IELogger::IELogger_Priority::CRITICAL); (::IE::IELogger::IsoLogger::Critical(__LINE__, __FILE__, Message, __VA_ARGS__)); }
#define ISOLOGGER_FATAL(Message,...) { IE::IELogger::IsoLogger::SetPriority(IE::IELogger::IELogger_Priority::FATAL); (::IE::IELogger::IsoLogger::Fatal(__LINE__, __FILE__, Message, __VA_ARGS__)); }

//#ifdef IE_ENABLE_ASSERTS
//#define IE_APPLICATION_ASSERT(x, ...) { if(!(x)) {ISOLOGGER_FATAL(("IsoEngine Application Assertion Failed:", x, __VA_ARGS__); __debugbreak(); }  // _debugbreak() is Windows OS only.
//#define //IE_ENGINE_ASSERT(Message, x, ...) { if(!(x)) { IE::IELogger::IsoLogger::SetPriority(IE::IELogger::IELogger_Priority::FATAL); ISOLOGGER_FATAL(Message, x, __VA_ARGS__); _IE_DEBUGBREAK(); } }		// TODO: Implement assertion support for other OS.
//#endif
