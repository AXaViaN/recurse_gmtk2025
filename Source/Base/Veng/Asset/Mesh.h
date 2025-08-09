#ifndef _VENG_ASSET_MESH
#define _VENG_ASSET_MESH

#include <AX/Tool/Macro.h>
#include <Veng/Asset/AssetManager.h>

#include <vector>

namespace Veng::Asset
{
	class Mesh
	{
	public:
		enum class MeshType;
		enum class VertexAttribType;
		struct VertexAttribInfo;
		struct Info;

	public:
		static auto CreateAsset(const std::string& name, const Info& info) -> AssetMeta<Mesh>;

	public:
		Mesh(const Info& info);

		void Use() const;
		inline auto GetVertexCount() const -> unsigned int;
		inline auto GetDrawMode() const -> unsigned int;

	public:
		~Mesh() noexcept;
		AX_SemanticsDeleteCopy(Mesh)
		AX_SemanticsImplMove(Mesh, m_VAO, m_VBO, m_EBO, m_VertexCount, m_DrawMode)

	private:
		unsigned int m_VAO = 0u;
		unsigned int m_VBO = 0u;
		unsigned int m_EBO = 0u;
		unsigned int m_VertexCount = 0u;
		unsigned int m_DrawMode = 0u;
	};

	/***** IMPL *****/

	enum class Mesh::MeshType
	{
		Triangle,
		Line
	};
	enum class Mesh::VertexAttribType
	{
		Vec2,
		Vec3
	};
	struct Mesh::VertexAttribInfo
	{
		VertexAttribType Type;
		size_t Offset;
	};
	struct Mesh::Info
	{
		MeshType Type;
		void* VertexData;
		size_t VertexSize;
		size_t VertexCount;
		std::vector<VertexAttribInfo> VertexAttribInfo;
		std::vector<unsigned int> Indices;
	};

	inline auto Mesh::GetVertexCount() const -> unsigned int
	{
		return m_VertexCount;
	}
	inline auto Mesh::GetDrawMode() const -> unsigned int
	{
		return m_DrawMode;
	}
}

#endif // !_VENG_ASSET_MESH
