#ifndef _AX_TOOL_DATE
#define _AX_TOOL_DATE

#include <string>

namespace AX::Tool
{
	class Date
	{
	public:
		static auto GetDateTimeFormatted(const char* format) -> std::string;

		static auto GetTimeStamp() -> std::string;
		static auto GetDateTimeStamp() -> std::string;
		static auto GetDateTimeStampAlphanumeric() -> std::string;
	};
}

#endif // _AX_TOOL_DATE
