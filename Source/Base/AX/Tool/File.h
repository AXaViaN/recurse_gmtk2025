#ifndef _AX_TOOL_FILE
#define _AX_TOOL_FILE

#include <string>
#include <cstdio>

namespace AX::Tool
{
	class File
	{
	public:
		enum class FileOption;

	public:
		File() = default;
		File(const std::string& path, FileOption fileOption);

		void Write(const std::string& data);
		void WriteLine(const std::string& data);

		auto Read(size_t size) -> std::string;
		auto ReadTill(char delimiter) -> std::string;
		auto ReadLine() -> std::string;
		auto ReadAll() -> std::string;

		template<typename T>
		inline void WriteBinary(const T& value);
		template<typename T>
		inline void ReadBinary(T& value);

		void MoveHead(int delta);
		void MoveHeadToFront();
		void MoveHeadToEnd();

		auto GetHeadPosition() const -> size_t;

		void Flush();

		void Delete();
		void DeleteContents();

		auto IsEOF() const -> bool;
		inline auto IsValid() const -> bool;

	public:
		~File() noexcept;
		File(const File& other) = delete;
		File& operator=(const File& other) = delete;
		inline File(File&& other) noexcept;
		inline File& operator=(File&& other) noexcept;

	private:
		void WriteBinaryNumber(const void* value, size_t size);
		void ReadBinaryNumber(void* value, size_t size);

	private:
		std::string m_Path;
		FILE* m_FileHandle = nullptr;

		bool m_IsBinaryFile = false;
	};

	/***** IMPL *****/

	enum class File::FileOption
	{
		CreateBinary,
		OpenBinary,
		CreateText,
		OpenText
	};

	template<typename T>
	inline void File::WriteBinary(const T& value)
	{
		WriteBinaryNumber(&value, sizeof(T));
	}
	template<typename T>
	inline void File::ReadBinary(T& value)
	{
		ReadBinaryNumber(&value, sizeof(T));
	}

	inline auto File::IsValid() const -> bool
	{
		return (m_FileHandle != nullptr);
	}

	inline File::File(File&& other) noexcept :
		m_Path(other.m_Path),
		m_FileHandle(other.m_FileHandle),
		m_IsBinaryFile(other.m_IsBinaryFile)
	{
		other.m_Path = "";
		other.m_FileHandle = nullptr;
	}
	inline File& File::operator=(File&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}

		m_Path = other.m_Path;
		m_FileHandle = other.m_FileHandle;
		m_IsBinaryFile = other.m_IsBinaryFile;
		other.m_Path = "";
		other.m_FileHandle = nullptr;

		return *this;
	}
}

#endif // !_AX_TOOL_FILE
