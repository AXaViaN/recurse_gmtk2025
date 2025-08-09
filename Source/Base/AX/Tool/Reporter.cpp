#include <AX/Tool/Reporter.h>

#include <AX/Tool/Date.h>
#include <AX/Tool/File.h>

#include <cstdio>
#include <string>
#include <map>
#include <vector>
#include <mutex>

#if AX_PLATFORM_WINDOWS
	#include <Windows.h>
#endif // AX_PLATFORM_WINDOWS

namespace AX::Tool
{
	class ReporterHelper
	{
	public:
		File LogFile;
		static constexpr auto MAX_LOG_FILESIZE = 50ull * 1024 * 1024; // 50 MB

		std::mutex LogMutex;

	public:
		ReporterHelper();

		void InitConsoleAttributes();
		void UseNextAttribute(Reporter::LogType logType);
		void UseDefaultAttribute();

#if AX_PLATFORM_WINDOWS
	private:
		static constexpr size_t AttributeCount = 2;
		static constexpr WORD ConsoleColorDefault = 15;
		static constexpr WORD ConsoleColorDebug1 = 10;
		static constexpr WORD ConsoleColorDebug2 = 14;
		static constexpr WORD ConsoleColorInfo1 = 9;
		static constexpr WORD ConsoleColorInfo2 = 11;
		static constexpr WORD ConsoleColorError1 = 4;
		static constexpr WORD ConsoleColorError2 = 5;
		static constexpr WORD ConsoleColorFatal1 = 15 + (4 * 0x10);
		static constexpr WORD ConsoleColorFatal2 = 15 + (5 * 0x10);

	private:
		std::map<Reporter::LogType, std::vector<WORD>> m_ConsoleAttributes;
		size_t m_CurrentConsoleAttributeIdx;
		HANDLE m_ConsoleHandle;
#endif // AX_PLATFORM_WINDOWS
	};

	static ReporterHelper s_Helper;

	/***** CLASS IMPL *****/

	void Reporter::InitLogFile(const std::string& logFilePath, const std::string& tag, bool shouldAppendDateToPath/*=false*/)
	{
		if(s_Helper.LogFile.IsValid())
		{
			AX_LogError("Logger is already initialized!");
			return;
		}

		auto logFileFullPath = logFilePath;
		if (shouldAppendDateToPath)
		{
			logFileFullPath += "_" + Date::GetDateTimeStampAlphanumeric() + ".log";
		}

		s_Helper.LogFile = File(logFileFullPath, File::FileOption::CreateText);
		
		auto logMessage = std::string("\n-------------------------------------------------------\n");
		logMessage += tag + " - " + Date::GetDateTimeStamp() + "\n\n";

		std::printf("%s", logMessage.c_str());
		if(s_Helper.LogFile.IsValid())
		{
			auto currentSize = s_Helper.LogFile.GetHeadPosition();
			if (currentSize >= s_Helper.MAX_LOG_FILESIZE)
			{
				s_Helper.LogFile.DeleteContents();
			}

			s_Helper.LogFile.Write(logMessage);
			s_Helper.LogFile.Flush();
		}
	}
	void Reporter::Log(const std::string& function, size_t line, LogType type, const std::string& message)
	{
		auto lock = std::unique_lock(s_Helper.LogMutex);

		static auto labelNames = std::map<LogType, std::string>{
			{LogType::Debug, "Debug"},
			{LogType::Info, "Info"},
			{LogType::Error, "Error"},
			{LogType::Fatal, "Fatal"}
		};

		auto logStr = '[' + Date::GetTimeStamp() + " - " + labelNames[type] + "] " + message;

		s_Helper.UseNextAttribute(type);
		std::printf("%s", logStr.c_str());
		s_Helper.UseDefaultAttribute();
		std::printf("\n");
		
		if(s_Helper.LogFile.IsValid())
		{
			s_Helper.LogFile.Write(
				'{' + function + "::" + std::to_string(line) + "}\t" + logStr + '\n'
			);
			s_Helper.LogFile.Flush();
		}
	}
	void Reporter::CrashReport(const std::string& /*function*/, size_t /*line*/, const std::string& /*message*/)
	{
		// TODO_ASC: Should have a crash report for AX_CONFIG_SHIPPING instead of just AX_Abort()
		AX_Abort();
	}

	/***** HELPER IMPL *****/

	ReporterHelper::ReporterHelper()
	{
		InitConsoleAttributes();
	}

	void ReporterHelper::InitConsoleAttributes()
	{
#if AX_PLATFORM_WINDOWS
		m_ConsoleAttributes = std::map<Reporter::LogType, std::vector<WORD>>{
			{Reporter::LogType::Debug,	{ConsoleColorDebug1, ConsoleColorDebug2}},
			{Reporter::LogType::Info,	{ConsoleColorInfo1, ConsoleColorInfo2}},
			{Reporter::LogType::Error,	{ConsoleColorError1, ConsoleColorError2}},
			{Reporter::LogType::Fatal,	{ConsoleColorFatal1, ConsoleColorFatal2}}
		};

		m_ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
#endif // AX_PLATFORM_WINDOWS
	}

	void ReporterHelper::UseNextAttribute(Reporter::LogType logType)
	{
#if AX_PLATFORM_WINDOWS
		SetConsoleTextAttribute(m_ConsoleHandle, m_ConsoleAttributes.at(logType).at(m_CurrentConsoleAttributeIdx++));

		if(m_CurrentConsoleAttributeIdx >= AttributeCount)
		{
			m_CurrentConsoleAttributeIdx = 0u;
		}
#endif // AX_PLATFORM_WINDOWS
	}
	void ReporterHelper::UseDefaultAttribute()
	{
#if AX_PLATFORM_WINDOWS
		SetConsoleTextAttribute(m_ConsoleHandle, ConsoleColorDefault);
#endif // AX_PLATFORM_WINDOWS
	}
}
