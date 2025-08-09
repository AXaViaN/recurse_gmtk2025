#ifndef _AX_TOOL_NUMBERCONVERTER
#define _AX_TOOL_NUMBERCONVERTER

#include <AX/Tool/Macro.h>

namespace AX::Tool
{
	class NumberConverter
	{
	public:
		template<typename T>
		inline static void ReverseBits(T& value);

	private:
		static void ReverseBits(uint8_t* value, size_t byteSize);
	};

	/***** IMPL *****/

	template<typename T>
	inline void NumberConverter::ReverseBits(T& value)
	{
		ReverseBits(reinterpret_cast<uint8_t*>(&value), sizeof(T));
	}
}

#endif // !_AX_TOOL_NUMBERCONVERTER
