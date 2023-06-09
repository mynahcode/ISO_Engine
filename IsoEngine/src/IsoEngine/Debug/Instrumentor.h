#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

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
		void BeginSession(const std::string& name, const std::string& filepath = "results.json")
		{
			m_OutputStream.open(filepath);
			WriteHeader();
			m_CurrentSession = new InstrumentationSession{ name };
		}

		void EndSession()
		{
			WriteFooter();
			m_OutputStream.close();
			delete m_CurrentSession;
			m_CurrentSession = nullptr;
			m_ProfileCount = 0;
		}

		void WriteProfile(const ProfilerResult& result)
		{
			if (m_ProfileCount++ > 0)
				m_OutputStream << ",";

			std::string name = result.ProfilerName;
			std::replace(name.begin(), name.end(), '"', '\'');

			m_OutputStream << "{";
			m_OutputStream << "\"cat\":\"function\",";
			m_OutputStream << "\"dur\":" << (result.EndTime - result.StartTime) << ',';
			m_OutputStream << "\"name\":\"" << name << "\",";
			m_OutputStream << "\"ph\":\"X\",";
			m_OutputStream << "\"pid\":0,";
			m_OutputStream << "\"tid\":" << result.ThreadID << ",";
			m_OutputStream << "\"ts\":" << result.EndTime;
			m_OutputStream << "}";

			m_OutputStream.flush();
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

		static Instrumentor& Get()
		{
			static Instrumentor instance;
			return instance;
		}

	private:
		InstrumentationSession* m_CurrentSession;
		std::ofstream m_OutputStream;
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
}

#define _IE_PROFILER 1
#if _IE_PROFILER

	#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
	#define _IE_FUNC_SIG __PRETTY_FUNCTION__
	#elif defined(__DMC__) && (__DMC__ >= 0x810)
	#define _IE_FUNC_SIG __PRETTY_FUNCTION__
	#elif defined(__FUNCSIG__)
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
	#define _IE_FUNC_SIG "_IE_FUNC_SIG unknown!"
	#endif

#define _IE_PROFILER_BEGIN_SESSION(name, filepath) ::IE::Instrumentor::Get().BeginSession(name, filepath)
#define _IE_PROFILER_END_SESSION() ::IE::Instrumentor::Get().EndSession()
#define _IE_PROFILER_SCOPE(name) ::IE::InstrumentationTimer timer##__LINE__(name);
#define _IE_PROFILER_FUNCTION() _IE_PROFILER_SCOPE(_IE_FUNC_SIG)

#else
#define _IE_PROFILER_BEGIN_SESSION(name, filepath)
#define _IE_PROFILER_END_SESSION()
#define _IE_PROFILER_SCOPE(name)
#define _IE_PROFILER_FUNCTION()

#endif
