#ifndef _AX_TOOL_STRINGCONVERTER
#define _AX_TOOL_STRINGCONVERTER

#include <string>
#include <cstdint>

namespace AX::Tool
{
	class StringConverter
	{
	public:
		static auto ToU64(const std::string& str) -> uint64_t;
		static auto ToF32(const std::string& str) -> float;
	};
}

#endif // !_AX_TOOL_STRINGCONVERTER
