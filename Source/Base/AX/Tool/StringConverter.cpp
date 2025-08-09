#include <AX/Tool/StringConverter.h>

#include <AX/Tool/Reporter.h>

namespace AX::Tool
{
	auto StringConverter::ToU64(const std::string& str) -> uint64_t
	{
		auto strEnd = &str[str.size()];
		auto nextPtr = (char*)nullptr;
		auto result = std::strtoull(str.c_str(), &nextPtr, 10);
		AX_SafeAssert(nextPtr == strEnd, "Invalid input");

		return result;
	}
	auto StringConverter::ToF32(const std::string& str) -> float
	{
		auto strEnd = &str[str.size()];
		auto nextPtr = (char*)nullptr;
		auto result = std::strtof(str.c_str(), &nextPtr);
		AX_SafeAssert(nextPtr == strEnd, "Invalid input");

		return result;
	}
}
