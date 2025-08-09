#include <AX/Tool/File.h>

#include <AX/Tool/Reporter.h>
#include <AX/Tool/PathManager.h>

#include <algorithm>

#if AX_PLATFORM_WINDOWS
	#include <Windows.h>
#endif // AX_PLATFORM_WINDOWS

namespace AX::Tool
{
	class FileHelper
	{
	public:
		void CreateFolders(const std::string& path);
		auto DoesFolderExists(const std::string& path) -> bool;
	};

	static FileHelper s_Helper;

	/***** CLASS IMPL *****/

	File::File(const std::string& path, FileOption fileOption) :
		m_Path(PathManager::GetFullPath(path))
	{
		if (m_Path == "")
		{
			return;
		}

		// Extract file mode from file options
		auto fileMode = std::string("ab+");

		m_IsBinaryFile =
			fileOption == FileOption::CreateBinary
			|| fileOption == FileOption::OpenBinary;
		auto isCreating =
			fileOption == FileOption::CreateBinary
			|| fileOption == FileOption::CreateText;

		if (m_IsBinaryFile == false)
		{
			fileMode = "a+";
		}
		if (isCreating == false)
		{
			fileMode[0] = 'r';
		}

		// Fix path slashes to use forward slashes
		std::replace(m_Path.begin(), m_Path.end(), '\\', '/');

		// Check if the path is a folder
		if (*(m_Path.rbegin()) == '/')
		{
			AX_LogError("Cannot open \"{}\", it is a folder!", m_Path);
			return;
		}

		m_FileHandle = std::fopen(m_Path.c_str(), fileMode.c_str());
		if (m_FileHandle == nullptr)
		{
			if (isCreating == false)
			{
				return;
			}

			// Check if it is a folder
			if (s_Helper.DoesFolderExists(m_Path))
			{
				AX_LogError("Cannot open \"{}\", it is a folder!", m_Path);
				return;
			}

			// Create the folders on the path
			size_t folderPathIdx = m_Path.find_last_of('/');
			if (folderPathIdx != m_Path.npos)
			{
				s_Helper.CreateFolders(m_Path.substr(0, folderPathIdx));
			}

			// Retry opening
			fileMode[0] = 'w';
			m_FileHandle = std::fopen(m_Path.c_str(), fileMode.c_str());
			if (m_FileHandle == nullptr)
			{
				AX_LogError("Cannot open \"{}\"", m_Path);
				return;
			}
		}

		// Move head for consistency
		MoveHeadToEnd();
	}

	void File::Write(const std::string& data)
	{
		if (m_IsBinaryFile)
		{
			std::fwrite(data.data(), sizeof(char), data.size(), m_FileHandle);
		}
		else
		{
			std::fprintf(m_FileHandle, "%s", data.c_str());
		}
	}
	void File::WriteLine(const std::string& data)
	{
		Write(data + "\n");
	}

	auto File::Read(size_t size) -> std::string
	{
		// TODO_ASC: Add proper large file support
		constexpr auto CHUNK_SIZE = 8 * 1024ull;

		auto data = std::string(size, '\0');
		auto readDataSize = 0ull;
		while (readDataSize < size)
		{
			auto remainingDataSize = size - readDataSize;
			auto toBeReadSize = (remainingDataSize < CHUNK_SIZE) ? remainingDataSize : CHUNK_SIZE;

			auto offset = sizeof(char) * readDataSize;
			auto currentReadSize = std::fread(
				data.data() + offset, sizeof(char), toBeReadSize, m_FileHandle
			);
			readDataSize += currentReadSize;

			if (IsEOF())
			{
				break;
			}
			AX_Assert(currentReadSize == toBeReadSize, "File read error: Cannot read chunk!");
		}
		
		return data;
	}
	auto File::ReadTill(char delimiter) -> std::string
	{
		auto data = std::string();
		while (true)
		{
			auto c = '\0';
			std::fread(&c, sizeof(char), 1u, m_FileHandle);
			if (IsEOF() || c == delimiter)
			{
				break;
			}

			data += c;
		}

		return data;
	}
	std::string File::ReadLine()
	{
		std::string data;
		while (true)
		{
			char c;
			std::fread(&c, sizeof(char), 1u, m_FileHandle);
			if (std::feof(m_FileHandle)
				|| c == '\n')
			{
				break;
			}
			else if (c == '\r')
			{
				continue;
			}

			data += c;
		}

		return data;
	}
	auto File::ReadAll() -> std::string
	{
		MoveHeadToEnd();
		auto size = GetHeadPosition();
		MoveHeadToFront();

		auto contents = Read(size);

		if (m_IsBinaryFile == false)
		{
			while (contents.back() == '\0')
			{
				contents.pop_back();
			}
		}

		return contents;
	}

	void File::MoveHead(int delta)
	{
		std::fseek(m_FileHandle, delta, SEEK_CUR);
	}
	void File::MoveHeadToFront()
	{
		std::fseek(m_FileHandle, 0, SEEK_SET);
	}
	void File::MoveHeadToEnd()
	{
		std::fseek(m_FileHandle, 0, SEEK_END);
	}

	auto File::GetHeadPosition() const -> size_t
	{
		return std::ftell(m_FileHandle);
	}

	void File::Flush()
	{
		std::fflush(m_FileHandle);
	}

	void File::Delete()
	{
		std::fclose(m_FileHandle);
		m_FileHandle = nullptr;

		std::remove(m_Path.c_str());
		m_Path = "";
	}
	void File::DeleteContents()
	{
		std::fclose(m_FileHandle);

		// Delete by re-opening with the writing mode
		auto fileMode = std::string("wb+");
		if (m_IsBinaryFile == false)
		{
			fileMode = "w+";
		}

		m_FileHandle = std::fopen(m_Path.c_str(), fileMode.c_str());
	}

	auto File::IsEOF() const -> bool
	{
		return std::feof(m_FileHandle);
	}

	File::~File() noexcept
	{
		if (m_FileHandle)
		{
			std::fclose(m_FileHandle);
			m_FileHandle = nullptr;
		}
	}

	void File::WriteBinaryNumber(const void* value, size_t size)
	{
		std::fwrite(value, size, 1u, m_FileHandle);
	}
	void File::ReadBinaryNumber(void* value, size_t size)
	{
		std::fread(value, size, 1u, m_FileHandle);
	}

	/***** HELPER IMPL *****/

	void FileHelper::CreateFolders(const std::string& path)
	{
		// TODO_ASC: Convert to std::filesystem

#if AX_PLATFORM_WINDOWS
		std::string folderPath = path;
		// Remove the last slash
		while (*(folderPath.rbegin()) == '/')
		{
			folderPath.erase(folderPath.end() - 1);
		}

		if (DoesFolderExists(folderPath) == false)
		{
			size_t lastSlash = folderPath.find_last_of('/');
			if (lastSlash != folderPath.npos)
			{
				// Recursively check if the folder exists. After this method,
				// we know that the folders before the slash are existing.
				CreateFolders(folderPath.substr(0, lastSlash));
			}

			// Create the folder
			CreateDirectoryA(folderPath.c_str(), nullptr);
		}
#elif AX_PLATFORM_WEB
		// No folder creation
#else
	#error No definition found for this OS
#endif // AX_PLATFORM_***
	}
	auto FileHelper::DoesFolderExists(const std::string& path) -> bool
	{
		// TODO_ASC: Convert to std::filesystem
		
		bool result = false;

#if AX_PLATFORM_WINDOWS
		// Check if folder exists
		DWORD dwAttrib = GetFileAttributesA(path.c_str());

		result =
			dwAttrib != INVALID_FILE_ATTRIBUTES
			&& (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
#elif AX_PLATFORM_WEB
		return false;
#else
	#error No definition found for this OS
#endif // AX_PLATFORM_***

		return result;
	}
}
