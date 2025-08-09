#include <AX/Ext/Thread.h>

#include <AX/Tool/Reporter.h>

#if AX_PLATFORM_WINDOWS
	#include <Windows.h>
#endif // AX_PLATFORM_WINDOWS

namespace AX::Ext
{
	auto Thread::GetCoreCount() -> size_t
	{
#if AX_PLATFORM_WINDOWS
		auto systemInfo = SYSTEM_INFO();
		GetSystemInfo(&systemInfo);
		return static_cast<size_t>(systemInfo.dwNumberOfProcessors);
#elif AX_PLATFORM_WEB
		return 1;
#else
	// Linux:
	/*
	#include <unistd.h>

	sysconf(_SC_NPROCESSORS_ONLN);
	*/
	#error No definition found for this OS
#endif // AX_PLATFORM_***
	}

	auto Thread::GetCurrentThreadID() -> size_t
	{
#if AX_PLATFORM_WINDOWS
		return static_cast<size_t>(GetCurrentThreadId());
#elif AX_PLATFORM_WEB
		return 0;
#else
	// Linux:
	/*
	#include <sys/types.h>
	
	pid_t gettid(void);
	*/
	#error No definition found for this OS
#endif // AX_PLATFORM_***
	}

	void Thread::SetAffinity(std::thread& thread, size_t core)
	{
#if AX_PLATFORM_WINDOWS
		auto result = SetThreadAffinityMask(thread.native_handle(), DWORD_PTR(1) << core);
		if (result == 0)
		{
			auto error = GetLastError();
			AX_LogError("Failed to set thread affinity for core {}: GLE = {}", core, error);
		}
#elif AX_PLATFORM_WEB
		// No affinity
#else
	// Linux:
	/*
	#include <pthread.h>

	pthread_t thread = pthread_self();
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(coreId, &cpuset);
    pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
	*/
	#error No definition found for this OS
#endif // AX_PLATFORM_***
	}
}
