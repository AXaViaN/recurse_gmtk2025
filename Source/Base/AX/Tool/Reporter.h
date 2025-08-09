#ifndef _AX_TOOL_REPORTER
#define _AX_TOOL_REPORTER

#include <AX/Tool/Macro.h>

#include <string>
#include <format>

#define AX_LogDebug(...)	AX::Tool::Reporter::Log(__FUNCTION__, __LINE__, AX::Tool::Reporter::LogType::Debug, __VA_ARGS__)
#define AX_LogInfo(...)		AX::Tool::Reporter::Log(__FUNCTION__, __LINE__, AX::Tool::Reporter::LogType::Info, __VA_ARGS__)
#define AX_LogError(...)	AX::Tool::Reporter::Log(__FUNCTION__, __LINE__, AX::Tool::Reporter::LogType::Error, __VA_ARGS__)
#define AX_LogFatal(...)	AX::Tool::Reporter::Log(__FUNCTION__, __LINE__, AX::Tool::Reporter::LogType::Fatal, __VA_ARGS__)

#if !(AX_CONFIG_DEBUG)
	#undef AX_LogDebug
	#define AX_LogDebug(...)
#endif // !(AX_CONFIG_DEBUG)

#define AX_INTERNAL_Assert_Log(arg1, arg2, message)		AX_LogFatal(message)
#define AX_INTERNAL_Assert_CrashReport(...)				AX::Tool::Reporter::CrashReport(__VA_ARGS__)
#define AX_INTERNAL_Assert_HandleSilentAssert(...)		AX_INTERNAL_MACRO_EXPAND(AX_INTERNAL_Assert_Log(__VA_ARGS__);)
#define AX_INTERNAL_Assert_HandleSafeAssert(...)		AX_INTERNAL_Assert_HandleSilentAssert(__VA_ARGS__) AX_DebugBreak();
#define AX_INTERNAL_Assert_HandleAssert(...)			AX_INTERNAL_Assert_HandleSafeAssert(__VA_ARGS__) AX_INTERNAL_Assert_CrashReport(__VA_ARGS__);
#define AX_INTERNAL_Assert_Assertion(assert_handle, assertion, message)		if(!(assertion)) { assert_handle(__FUNCTION__, __LINE__, message) }

#define AX_INTERNAL_Assert_NO_MESSAGE(assertion)				AX_INTERNAL_Assert_Assertion(AX_INTERNAL_Assert_HandleAssert, assertion, std::format("ASSERTION FAILED: ({})", #assertion))
#define AX_INTERNAL_Assert_MESSAGE(assertion, message)			AX_INTERNAL_Assert_Assertion(AX_INTERNAL_Assert_HandleAssert, assertion, message)
#define AX_Assert(...) AX_INTERNAL_MACRO_EXPAND( AX_INTERNAL_CONDITIONAL_MACRO_MAX_PARAM_COUNT_2(__VA_ARGS__, AX_INTERNAL_Assert_MESSAGE, AX_INTERNAL_Assert_NO_MESSAGE) (__VA_ARGS__) )

#define AX_INTERNAL_SafeAssert_NO_MESSAGE(assertion)			AX_INTERNAL_Assert_Assertion(AX_INTERNAL_Assert_HandleSafeAssert, assertion, std::format("Safe Assertion Failed: ({})", #assertion))
#define AX_INTERNAL_SafeAssert_MESSAGE(assertion, message)		AX_INTERNAL_Assert_Assertion(AX_INTERNAL_Assert_HandleSafeAssert, assertion, message)
#define AX_SafeAssert(...) AX_INTERNAL_MACRO_EXPAND( AX_INTERNAL_CONDITIONAL_MACRO_MAX_PARAM_COUNT_2(__VA_ARGS__, AX_INTERNAL_SafeAssert_MESSAGE, AX_INTERNAL_SafeAssert_NO_MESSAGE) (__VA_ARGS__) )

#define AX_INTERNAL_SilentAssert_NO_MESSAGE(assertion)			AX_INTERNAL_Assert_Assertion(AX_INTERNAL_Assert_HandleSilentAssert, assertion, std::format("Silent Assertion Failed: ({})", #assertion))
#define AX_INTERNAL_SilentAssert_MESSAGE(assertion, message)	AX_INTERNAL_Assert_Assertion(AX_INTERNAL_Assert_HandleSilentAssert, assertion, message)
#define AX_SilentAssert(...) AX_INTERNAL_MACRO_EXPAND( AX_INTERNAL_CONDITIONAL_MACRO_MAX_PARAM_COUNT_2(__VA_ARGS__, AX_INTERNAL_SilentAssert_MESSAGE, AX_INTERNAL_SilentAssert_NO_MESSAGE) (__VA_ARGS__) )

namespace AX::Tool
{
	class Reporter
	{
	public:
		enum class LogType;

	public:
		static void InitLogFile(const std::string& logFilePath, const std::string& tag, bool shouldAppendDateToPath=false);

		static void Log(const std::string& function, size_t line, LogType type, const std::string& message);
		template<typename... Args>
		static inline void Log(const std::string& function, size_t line, LogType type, std::format_string<Args...> fmt, Args&&... args);

		static void CrashReport(const std::string& function, size_t line, const std::string& message);
	};

	/***** IMPL *****/

	enum class Reporter::LogType
	{
		Debug,
		Info,
		Error,
		Fatal
	};

	template<typename... Args>
	inline void Reporter::Log(const std::string& function, size_t line, LogType type, std::format_string<Args...> fmt, Args&&... args)
	{
		Log(function, line, type, std::format(fmt, std::forward<Args>(args)...));
	}
}

#endif // !_AX_TOOL_REPORTER
