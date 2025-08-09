#include <AX/Tool/PathManager.h>

#include <AX/Tool/Reporter.h>

#include <filesystem>

namespace AX::Tool
{
	static std::string g_DistroDir;

	void PathManager::Initialize(int /*argc*/, char** argv)
	{
#if AX_PLATFORM_WEB
		// Paths are already virtual
		return;
#endif

		// TODO_ASC: Needs backslash conversion depending on platform
		g_DistroDir = argv[0];
		auto backslashCount = 3;
		auto lastBackslashIdx = 0ull;
		for (auto i = g_DistroDir.size() - 1; i > 0; --i)
		{
			if (g_DistroDir[i] == '\\')
			{
				if (--backslashCount == 0)
				{
					lastBackslashIdx = i;
					break;
				}
			}
		}
		g_DistroDir = g_DistroDir.substr(0, lastBackslashIdx + 1);

		AX_LogDebug("App root folder: \"{}\"", g_DistroDir);
	}

	auto PathManager::GetFullPath(const std::string& path) -> std::string
	{
		return g_DistroDir + path;
	}

	auto PathManager::GetFilePaths(const std::string& path) -> std::vector<std::string>
	{
		auto filePaths = std::vector<std::string>();

		auto fullPath = GetFullPath(path);
		if (std::filesystem::exists(fullPath) == false)
		{
			return {};
		}

		for (auto& entry : std::filesystem::recursive_directory_iterator(fullPath))
		{
			if (entry.is_directory())
			{
				continue;
			}

			auto entryFullPath = entry.path().string();
			auto entryRelativePath = entryFullPath.substr(fullPath.size() - path.size());

			filePaths.emplace_back(entryRelativePath);
		}

		return filePaths;
	}
}
