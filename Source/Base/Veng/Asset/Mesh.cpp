#include <Veng/Asset/Mesh.h>

#include <AX/Tool/Reporter.h>

#include <glm/glm.hpp>
#include <glax/gl.h>

#include <unordered_map>

namespace Veng::Asset
{
	auto Mesh::CreateAsset(const std::string& name, const Info& info) -> AssetMeta<Mesh>
	{
		return AssetManager::CreateAsset(name, Mesh(info));
	}

	Mesh::Mesh(const Info& info)
	{
		m_VertexCount = static_cast<unsigned int>(info.Indices.size());
		if (info.Type == MeshType::Triangle)
		{
			m_DrawMode = GL_TRIANGLES;
		}
		else if (info.Type == MeshType::Line)
		{
			m_DrawMode = GL_LINES;
		}

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, info.VertexSize * info.VertexCount, info.VertexData, GL_STATIC_DRAW);

		for (auto i = 0; i < info.VertexAttribInfo.size(); ++i)
		{
			auto& vertexAttribInfo = info.VertexAttribInfo[i];
			auto size = 0;
			if (vertexAttribInfo.Type == VertexAttribType::Vec2)
			{
				size = 2;
			}
			else if (vertexAttribInfo.Type == VertexAttribType::Vec3)
			{
				size = 3;
			}

			glEnableVertexAttribArray(i);
			glVertexAttribPointer(
				i, size, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(info.VertexSize),
				reinterpret_cast<const void*>(vertexAttribInfo.Offset)
			);
		}

		glGenBuffers(1, &m_EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * info.Indices.size(), info.Indices.data(), GL_STATIC_DRAW);
	}

	void Mesh::Use() const
	{
		glBindVertexArray(m_VAO);
	}

	Mesh::~Mesh() noexcept
	{
		if (m_VAO != 0u)
		{
			glDeleteBuffers(1, &m_EBO);
			glDeleteBuffers(1, &m_VBO);
			glDeleteVertexArrays(1, &m_VAO);

			m_VAO = 0u;
			m_VBO = 0u;
			m_EBO = 0u;
			m_VertexCount = 0u;
		}
	}
}
