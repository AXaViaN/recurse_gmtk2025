#include <AX/Tool/NumberConverter.h>

#include <AX/Tool/Reporter.h>

namespace AX::Tool
{
	void NumberConverter::ReverseBits(uint8_t* value, size_t byteSize)
	{
		AX_Assert(byteSize <= 8, "NumberConverter::ReverseBits() supports 64 bits max");

		uint8_t original[8];
		for (auto byteIdx = 0; byteIdx < byteSize; ++byteIdx)
		{
			original[byteIdx] = value[byteIdx];
			value[byteIdx] = 0;
		}

		for (auto byteIdx = 0; byteIdx < byteSize; ++byteIdx)
		{
			auto& org = original[byteIdx];
			auto& val = value[byteSize - byteIdx - 1];
			for (auto i = 0; i < 4; ++i)
			{
				val |= (org & (0x01 << i)) << (7 - i * 2);
				val |= (org & (0x01 << (7 - i))) >> (7 - i * 2);
			}
		}
	}
}
