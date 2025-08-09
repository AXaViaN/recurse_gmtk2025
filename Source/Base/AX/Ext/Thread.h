#ifndef _AX_EXT_THREAD
#define _AX_EXT_THREAD

#include <thread>

namespace AX::Ext
{
	class Thread
	{
	public:
		static auto GetCoreCount() -> size_t;

		static auto GetCurrentThreadID() -> size_t;

		static void SetAffinity(std::thread& thread, size_t core);
	};
}

#endif // !_AX_EXT_THREAD
