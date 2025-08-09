#include <AX/Tool/Date.h>

#include <chrono>

namespace AX::Tool
{
	auto Date::GetDateTimeFormatted(const char* format) -> std::string
	{
		auto now = std::chrono::system_clock::now();
		auto now_c = std::chrono::system_clock::to_time_t(now);
		auto tmDate = std::localtime(&now_c);

		char buffer[256];
		std::strftime(buffer, sizeof(buffer), format, tmDate);

		return buffer;
	}

	auto Date::GetTimeStamp() -> std::string
	{
		auto duration = std::chrono::system_clock::now().time_since_epoch();
		auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() % 1000;
		auto time = GetDateTimeFormatted("%H:%M:%S");

		char buffer[256];
		std::snprintf(buffer, sizeof(buffer), "%s:%03lld", time.c_str(), millis);

		return buffer;
	}
	auto Date::GetDateTimeStamp() -> std::string
	{
		return GetDateTimeFormatted("%Y.%m.%d %H:%M:%S");
	}
	auto Date::GetDateTimeStampAlphanumeric() -> std::string
	{
		return GetDateTimeFormatted("%Y%m%d_%H%M%S");
	}
}