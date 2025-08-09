#ifndef _AX_TOOL_PATHMANAGER
#define _AX_TOOL_PATHMANAGER

#include <string>
#include <vector>

namespace AX::Tool
{
	class PathManager
	{
	public:
		static void Initialize(int argc, char** argv);
		static auto GetFullPath(const std::string& path) -> std::string;

		static auto GetFilePaths(const std::string& path) -> std::vector<std::string>;
	};
}

#endif // !_AX_TOOL_PATHMANAGER
