#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <mutex>

#include <thread>

// TODO: Add macros

namespace IE
{
	struct ProfilerResult
	{
		std::string ProfilerName;
		long long StartTime, EndTime;
		uint32_t ThreadID;
	};

	struct InstrumentationSession
	{
		std::string Name;
	};

	class Instrumentor
	{
	public:
		Instrumentor(const Instrumentor&) = delete;
		Instrumentor(Instrumentor&&) = delete;

		void BeginSession(const std::string& name, const std::string& filepath = "results.json")
		{
			std::lock_guard lock(m_Mutex);
			if (m_CurrentSession)
			{
				IELogger::IsoLogger::SetPriority(IELogger::IELogger_Priority::CRITICAL);
				ISOLOGGER_CRITICAL("Instrumentor::BeginSession('{%}') when session {%} already open.", name, m_CurrentSession->Name);

				InternalEndSession();
			}
			m_OutputStream.open(filepath);

			if (m_OutputStream.is_open())
			{
				m_CurrentSession = new InstrumentationSession({ name });
				WriteHeader();
			}
			else
			{
				IELogger::IsoLogger::SetPriority(IELogger::IELogger_Priority::CRITICAL);
				ISOLOGGER_CRITICAL("Instrumentor could not open results file {%}", filepath);
			}
		}

		void EndSession()
		{
			std::lock_guard lock(m_Mutex);
			InternalEndSession();
		}

		void WriteProfile(const ProfilerResult& result)
		{
			std::stringstream json;

			json << std::setprecision(3) << std::fixed;
			json << ",{";
			json << "\"cat\":\"function\",";
			json << "\"dur\":" << (result.EndTime - result.StartTime) << ',';
			json << "\"name\":\"" << result.ProfilerName << "\",";
			json << "\"ph\":\"X\",";
			json << "\"pid\":0,";
			json << "\"tid\":" << result.ThreadID << ",";
			json << "\"ts\":" << result.EndTime;
			json << "}";

			std::lock_guard lock(m_Mutex);
			if (m_CurrentSession)
			{
				m_OutputStream << json.str();
				m_OutputStream.flush();
			}
		}

		void WriteHeader()
		{
			m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
			m_OutputStream.flush();
		}

		void WriteFooter()
		{
			m_OutputStream << "]}";
			m_OutputStream.flush();
		}

		void InternalEndSession()
		{
			if (m_CurrentSession)
			{
				WriteFooter();
				m_OutputStream.close();
				delete m_CurrentSession;
				m_CurrentSession = nullptr;
			}
		}

		static Instrumentor& Get()
		{
			static Instrumentor instance;
			return instance;
		}

	private:

		Instrumentor()
			: m_CurrentSession(nullptr)
		{
		}
		~Instrumentor()
		{
			EndSession();
		}

		InstrumentationSession* m_CurrentSession;
		std::ofstream m_OutputStream;
		std::mutex m_Mutex;
		int m_ProfileCount;
	};

	class InstrumentationTimer
	{
	public:

		InstrumentationTimer(const char* name)
			: m_Name(name), m_Stopped(false)
		{
			m_StartTimepoint = std::chrono::high_resolution_clock::now();
		}

		~InstrumentationTimer()
		{
			if (!m_Stopped)
				Stop();
		}

		void Stop()
		{
			auto endTimepoint = std::chrono::high_resolution_clock::now();

			long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
			long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

			uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
			Instrumentor::Get().WriteProfile({ m_Name, start, end, threadID });

			m_Stopped = true;
		}
	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
		bool m_Stopped;
	};

	namespace InstrumentorUtils {

		template <size_t N>
		struct ChangeResult
		{
			char Data[N];
		};

		template <size_t N, size_t K>
		constexpr auto CleanupOutputString(const char(&expr)[N], const char(&remove)[K])
		{
			ChangeResult<N> result = {};

			size_t srcIndex = 0;
			size_t dstIndex = 0;
			while (srcIndex < N)
			{
				size_t matchIndex = 0;
				while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
					matchIndex++;
				if (matchIndex == K - 1)
					srcIndex += matchIndex;
				result.Data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
				srcIndex++;
			}
			return result;
		}
	}
}

#define _IE_PROFILER 1
#if _IE_PROFILER
// Resolve which function signature macro will be used. Note that this only
// is resolved when the (pre)compiler starts, so the syntax highlighting
// could mark the wrong one in your editor!
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define _IE_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define_IE_FUNC_SIG __PRETTY_FUNCTION__
#elif (defined(__FUNCSIG__) || (_MSC_VER))
#define _IE_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define _IE_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define _IE_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define _IE_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define _IE_FUNC_SIG __func__
#else
#define _IE_FUNC_SIG "HZ_FUNC_SIG unknown!"
#endif

#define _IE_PROFILER_BEGIN_SESSION(name, filepath) ::IE::Instrumentor::Get().BeginSession(name, filepath)
#define _IE_PROFILER_END_SESSION() ::IE::Instrumentor::Get().EndSession()
#define _IE_PROFILER_SCOPE_LINE2(name, line) constexpr auto fixedName##line = ::IE::InstrumentorUtils::CleanupOutputString(name, "__cdecl ");\
											   ::IE::InstrumentationTimer timer##line(fixedName##line.Data)
#define _IE_PROFILER_SCOPE_LINE(name, line) _IE_PROFILER_SCOPE_LINE2(name, line)
#define _IE_PROFILER_SCOPE(name) _IE_PROFILER_SCOPE_LINE(name, __LINE__)
#define _IE_PROFILER_FUNCTION() _IE_PROFILER_SCOPE(_IE_FUNC_SIG)
#else
#define _IE_PROFILE_BEGIN_SESSION(name, filepath)
#define _IE_PROFILE_END_SESSION()
#define _IE_PROFILE_SCOPE(name)
#define _IE_PROFILE_FUNCTION()
#endif
