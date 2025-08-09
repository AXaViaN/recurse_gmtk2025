#include <Veng/Asset/Shader.h>

#include <AX/Tool/File.h>
#include <AX/Tool/Reporter.h>
#include <AX/Tool/StringConverter.h>

#include <glax/gl.h>

namespace Veng::Asset
{
	class ShaderHelper
	{
	public:
		struct ParseShaderSourceResult
		{
			std::string VertexShaderSource;
			std::string FragmentShaderSource;

			std::unordered_map<std::string, int> UniformLocations;
			std::vector<Material::Property> MaterialProperties;
			std::unordered_map<std::string, size_t> MaterialPropertyNameToIdx;
		};

		enum class ShaderType
		{
			Vertex,
			Fragment
		};

	public:
		std::string_view PROPERTIES_KEY = std::string_view("Properties");
		std::string_view VERTEX_MAIN_KEY = std::string_view("VertexMain");
		std::string_view FRAGMENT_MAIN_KEY = std::string_view("FragmentMain");
		std::string_view ATTRIBUTE_KEY = std::string_view("attr_");
		std::string_view VARYING_KEY = std::string_view("vary_");
		std::string_view SAMPLER_KEY = std::string_view("samp_");
		std::string_view UNIFORM_KEY = std::string_view("unif_");
		std::string_view OUTPUT_KEY = std::string_view("outp_");

		std::string_view ENTRY_NAME = std::string_view("main");
		std::string_view VARYING_TYPE_VF_KEY = std::string_view("vf");

	public:
		auto ParseShaderSource(Shader* self, const std::string& path) -> ParseShaderSourceResult;
		auto GetScopeEnd(Shader* self, const std::string& source, size_t start) -> size_t;

		void ParseMaterialProperties(ParseShaderSourceResult& result, const std::string& propertyCode);

		auto CreateShader(ShaderType type, const ParseShaderSourceResult& parsedShaderSource) -> unsigned int;
		auto CreateShaderProgram(const ParseShaderSourceResult& parsedShaderSource, unsigned int vertexShader, unsigned int fragmentShader) -> unsigned int;

		auto GetCodeWithLineNumbers(const std::string& code) -> std::string;

		auto GetUniformLocation(Shader* self, const std::string& name) -> int;
	};

	static ShaderHelper s_Helper;

	/***** CLASS IMPL *****/

	auto Shader::CreateAsset(const std::string& path) -> AssetMeta<Shader>
	{
		return AssetManager::CreateAsset(path, Shader(path));
	}

	Shader::Shader(const std::string& path) :
		m_Name(path)
	{
		auto parsedShaderSource = s_Helper.ParseShaderSource(this, path);
		m_UniformLocations = std::move(parsedShaderSource.UniformLocations);
		m_MaterialProperties = std::move(parsedShaderSource.MaterialProperties);

		auto vertexShader = s_Helper.CreateShader(
			ShaderHelper::ShaderType::Vertex, parsedShaderSource
		);
		auto fragmentShader = s_Helper.CreateShader(
			ShaderHelper::ShaderType::Fragment, parsedShaderSource
		);

		m_ProgramID = s_Helper.CreateShaderProgram(parsedShaderSource, vertexShader, fragmentShader);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

#if AX_PLATFORM_WEB
		// Because there is no layout locations for uniforms on ES
		for (auto& uniformLocation : m_UniformLocations)
		{
			uniformLocation.second = glGetUniformLocation(m_ProgramID, ("unif_" + uniformLocation.first).c_str());
		}

		Use();
		for (auto& materialProperty : m_MaterialProperties)
		{
			if (materialProperty.Type != Material::PropertyType::Texture)
			{
				continue;
			}

			auto binding = s_Helper.GetUniformLocation(this, materialProperty.Name);
			SetUniform(materialProperty.Name, binding);
		}
#endif
	}

	void Shader::Use() const
	{
		glUseProgram(m_ProgramID);
	}

	void Shader::SetUniform(const std::string& name, int value)
	{
		auto location = s_Helper.GetUniformLocation(this, name);
		glUniform1i(location, value);
	}
	void Shader::SetUniform(const std::string& name, float value)
	{
		auto location = s_Helper.GetUniformLocation(this, name);
		glUniform1f(location, value);
	}
	void Shader::SetUniform(const std::string& name, const AssetMeta<Texture>& value)
	{
		auto location = s_Helper.GetUniformLocation(this, name);
		value->Use(location);
	}
	void Shader::SetUniform(const std::string& name, const glm::vec2& value)
	{
		auto location = s_Helper.GetUniformLocation(this, name);
		glUniform2fv(location, 1, &value[0]);
	}
	void Shader::SetUniform(const std::string& name, const glm::vec3& value)
	{
		auto location = s_Helper.GetUniformLocation(this, name);
		glUniform3fv(location, 1, &value[0]);
	}
	void Shader::SetUniform(const std::string& name, const glm::vec4& value)
	{
		auto location = s_Helper.GetUniformLocation(this, name);
		glUniform4fv(location, 1, &value[0]);
	}
	void Shader::SetUniform(const std::string& name, const glm::mat4& value)
	{
		auto location = s_Helper.GetUniformLocation(this, name);
		glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
	}

	auto Shader::GetRenderBackendVersion() -> const std::string&
	{
		static auto glslVersion = std::string();
		if (glslVersion.size() != 0)
		{
			return glslVersion;
		}

		auto glslVersionMajor = 0;
		auto glslVersionMinor = 0;
		glGetIntegerv(GL_MAJOR_VERSION, &glslVersionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &glslVersionMinor);

#if AX_PLATFORM_WEB
		auto glslProfile = "es";
#else
		auto glslProfile = "core";
#endif

		glslVersion = std::format(
			"#version {}{}0 {}",
			std::to_string(glslVersionMajor),
			std::to_string(glslVersionMinor),
			glslProfile
		);

		return glslVersion;
	}

	Shader::~Shader() noexcept
	{
		if (m_ProgramID != 0u)
		{
			glDeleteProgram(m_ProgramID);

			m_ProgramID = 0u;
		}
	}

	/***** HELPER IMPL *****/

	auto ShaderHelper::ParseShaderSource(Shader* self, const std::string& path) -> ParseShaderSourceResult
	{
		auto shaderSource = std::string();
		{
			auto shaderSourceFile = AX::Tool::File(path, AX::Tool::File::FileOption::OpenText);
			AX_Assert(shaderSourceFile.IsValid(), "Shader source cannot be read at: \"" + path + "\"");

			shaderSource = shaderSourceFile.ReadAll();
			shaderSource.erase(std::remove(shaderSource.begin(), shaderSource.end(), '\r'), shaderSource.end());
		}

		auto result = ParseShaderSourceResult();
		{
			auto versionStr = Shader::GetRenderBackendVersion() + "\n\n";
			result.VertexShaderSource += versionStr;
			result.FragmentShaderSource += versionStr;

#if AX_PLATFORM_WEB
			result.FragmentShaderSource += "precision mediump float;\n\n";
#endif
		}

		auto shaderSourceCursor = 0ull;
		while (shaderSourceCursor < shaderSource.size())
		{
			auto lineStart = shaderSourceCursor;
			auto lineEnd = shaderSource.find_first_of('\n', lineStart);

			auto line = shaderSource.substr(lineStart, lineEnd - lineStart);
			shaderSourceCursor = lineEnd + 1;

			if (line.contains(PROPERTIES_KEY))
			{
				auto scopeEnd = GetScopeEnd(self, shaderSource, lineStart);
				lineEnd = shaderSource.find_first_of('\n', scopeEnd);

				auto scopeCode = shaderSource.substr(lineStart, lineEnd - lineStart);
				ParseMaterialProperties(result, scopeCode);

				shaderSourceCursor = lineEnd + 1;
			}
			else if (line.contains(VERTEX_MAIN_KEY))
			{
				auto scopeEnd = GetScopeEnd(self, shaderSource, lineStart);
				lineEnd = shaderSource.find_first_of('\n', scopeEnd);

				auto scopeCode = shaderSource.substr(lineStart, lineEnd - lineStart) + "\n";
				auto entryNameStart = scopeCode.find(VERTEX_MAIN_KEY);
				auto entryNameEnd = entryNameStart + VERTEX_MAIN_KEY.size();
				scopeCode.replace(entryNameStart, entryNameEnd - entryNameStart, ENTRY_NAME);

				result.VertexShaderSource += scopeCode;
				shaderSourceCursor = lineEnd + 1;
			}
			else if (line.contains(FRAGMENT_MAIN_KEY))
			{
				auto scopeEnd = GetScopeEnd(self, shaderSource, lineStart);
				lineEnd = shaderSource.find_first_of('\n', scopeEnd);

				auto scopeCode = shaderSource.substr(lineStart, lineEnd - lineStart) + "\n";
				auto entryNameStart = scopeCode.find(FRAGMENT_MAIN_KEY);
				auto entryNameEnd = entryNameStart + FRAGMENT_MAIN_KEY.size();
				scopeCode.replace(entryNameStart, entryNameEnd - entryNameStart, ENTRY_NAME);

				result.FragmentShaderSource += scopeCode;
				shaderSourceCursor = lineEnd + 1;
			}
			else if (line.contains('{'))
			{
				auto scopeEnd = GetScopeEnd(self, shaderSource, lineStart);
				lineEnd = shaderSource.find_first_of('\n', scopeEnd);

				auto scopeCode = shaderSource.substr(lineStart, lineEnd - lineStart) + "\n";
				result.VertexShaderSource += scopeCode;
				result.FragmentShaderSource += scopeCode;
				shaderSourceCursor = lineEnd + 1;
			}
			else if (line.contains(ATTRIBUTE_KEY))
			{
				auto identifierStart = line.find(ATTRIBUTE_KEY);
				auto identifierEnd = line.find_first_of(' ', identifierStart);

				auto locationStart = identifierStart + ATTRIBUTE_KEY.size();
				auto locationStr = line.substr(locationStart, identifierEnd - locationStart);

				result.VertexShaderSource += (
					"layout(location = " + locationStr + ") in " +
					line.substr(identifierEnd + 1) + "\n"
				);
			}
			else if (line.contains(VARYING_KEY))
			{
				auto identifierStart = line.find(VARYING_KEY);
				auto identifierEnd = line.find_first_of(' ', identifierStart);

				auto typeStart = identifierStart + VARYING_KEY.size();
				auto typeStr = line.substr(typeStart, identifierEnd - typeStart);

				auto code = line.substr(identifierEnd + 1) + "\n";
				if (typeStr == VARYING_TYPE_VF_KEY)
				{
					result.VertexShaderSource += "out " + code;
					result.FragmentShaderSource += "in " + code;
				}
				else
				{
					AX_LogError("Syntax error while parsing varying in shader \"" + self->m_Name + "\"");
					break;
				}
			}
			else if (line.contains(SAMPLER_KEY))
			{
				auto identifierStart = line.find(SAMPLER_KEY);
				auto identifierEnd = line.find_first_of(' ', identifierStart);

				auto bindingStart = identifierStart + SAMPLER_KEY.size();
				auto bindingStr = line.substr(bindingStart, identifierEnd - bindingStart);

				auto definition = line.substr(identifierEnd + 1);
				auto code = (
#if AX_PLATFORM_WEB
					"uniform " +
#else
					"layout(binding = " + bindingStr + ") uniform " +
#endif
					definition + "\n"
				);
				result.VertexShaderSource += code;
				result.FragmentShaderSource += code;

				auto nameEnd = definition.find_first_of(';');
				auto nameStart = definition.find(UNIFORM_KEY) + UNIFORM_KEY.size();
				auto name = definition.substr(nameStart, nameEnd - nameStart);
				result.UniformLocations.emplace(
					name,
					static_cast<int>(AX::Tool::StringConverter::ToU64(bindingStr))
				);

				if (result.MaterialPropertyNameToIdx.contains(name))
				{
					auto& idx = result.MaterialPropertyNameToIdx.at(name);
					result.MaterialProperties.at(idx).Type = Material::PropertyType::Texture;
				}
			}
			else if (line.contains(UNIFORM_KEY))
			{
				auto identifierStart = line.find(UNIFORM_KEY);
				auto identifierEnd = line.find_first_of(' ', identifierStart);

				auto locationStart = identifierStart + UNIFORM_KEY.size();
				auto locationStr = line.substr(locationStart, identifierEnd - locationStart);

				auto definition = line.substr(identifierEnd + 1);
				auto code = (
#if AX_PLATFORM_WEB
					"uniform " +
#else
					"layout(location = " + locationStr + ") uniform " +
#endif
					definition + "\n"
				);
				result.VertexShaderSource += code;
				result.FragmentShaderSource += code;

				auto nameEnd = definition.find_first_of(';');
				auto nameStart = definition.find(UNIFORM_KEY) + UNIFORM_KEY.size();
				auto name = definition.substr(nameStart, nameEnd - nameStart);
				result.UniformLocations.emplace(
					name,
					static_cast<int>(AX::Tool::StringConverter::ToU64(locationStr))
				);

				if (result.MaterialPropertyNameToIdx.contains(name))
				{
					auto& idx = result.MaterialPropertyNameToIdx.at(name);

					auto typeNameStart = definition.find_first_not_of(" \t");
					auto typeNameEnd = definition.find_first_of(" \t", typeNameStart);
					auto typeName = definition.substr(typeNameStart, typeNameEnd - typeNameStart);
					if (typeName == "float")
					{
						result.MaterialProperties.at(idx).Type = Material::PropertyType::Float;
					}
					else if (typeName == "vec2")
					{
						result.MaterialProperties.at(idx).Type = Material::PropertyType::Vec2;
					}
					else if (typeName == "vec3")
					{
						result.MaterialProperties.at(idx).Type = Material::PropertyType::Vec3;
					}
					else if (typeName == "vec4")
					{
						result.MaterialProperties.at(idx).Type = Material::PropertyType::Vec4;
					}
					else
					{
						AX_Assert(false, "PropertyType for \"" + typeName + "\" not found!");
					}
				}
			}
			else if (line.contains(OUTPUT_KEY))
			{
				auto identifierStart = line.find(OUTPUT_KEY);
				auto identifierEnd = line.find_first_of(' ', identifierStart);

				auto locationStart = identifierStart + OUTPUT_KEY.size();
				auto locationStr = line.substr(locationStart, identifierEnd - locationStart);

				auto code = (
					"layout(location = " + locationStr + ") out " +
					line.substr(identifierEnd + 1) + "\n"
				);
				result.FragmentShaderSource += code;
			}
			else
			{
				line += "\n";
				result.VertexShaderSource += line;
				result.FragmentShaderSource += line;
			}
		}

		return result;
	}
	auto ShaderHelper::GetScopeEnd(Shader* self, const std::string& source, size_t start) -> size_t
	{
		auto scopeStart = source.find_first_of('{', start);
		auto sourceCursor = scopeStart + 1;
		auto curlyBraceCount = 1u;
		while (curlyBraceCount != 0u)
		{
			auto nextScopeStart = source.find_first_of('{', sourceCursor);
			auto nextScopeEnd = source.find_first_of('}', sourceCursor);
			if (nextScopeEnd == std::string::npos)
			{
				AX_LogError("Syntax error while parsing scope in shader \"" + self->m_Name + "\"");
				break;
			}
			else if (nextScopeStart < nextScopeEnd)
			{
				++curlyBraceCount;
				sourceCursor = nextScopeStart + 1;
			}
			else
			{
				--curlyBraceCount;
				sourceCursor = nextScopeEnd + 1;
			}
		}

		return sourceCursor;
	}

	void ShaderHelper::ParseMaterialProperties(ParseShaderSourceResult& result, const std::string& propertyCode)
	{
		auto listStart = propertyCode.find_first_of('{') + 1;
		auto listEnd = propertyCode.find_last_of('}');
		auto propertyCursor = listStart;
		while (propertyCursor < listEnd)
		{
			auto propertyStart = propertyCode.find_first_not_of(" \t\n", propertyCursor);
			auto propertyEnd = propertyCode.find_first_of("\n", propertyStart);
			propertyCursor = propertyEnd;
			if (propertyStart >= listEnd)
			{
				break;
			}

			auto propertyNameStart = propertyStart + UNIFORM_KEY.size();
			auto propertyNameEnd = propertyCode.find_first_of(" =[\t\n", propertyNameStart);
			auto propertyName = propertyCode.substr(propertyNameStart, propertyNameEnd - propertyNameStart);

			auto& property = result.MaterialProperties.emplace_back(Material::Property{
				.Name = propertyName
			});
			result.MaterialPropertyNameToIdx.emplace(
				propertyName, result.MaterialProperties.size() - 1
			);

			auto propertyRange = propertyCode.find_first_of('[', propertyNameEnd);
			if (propertyRange < propertyEnd)
			{
				auto rangeMinStart = propertyCode.find_first_not_of(" \t", propertyRange + 1);
				auto rangeMinEnd = propertyCode.find_first_of(",", rangeMinStart);
				auto rangeMinStr = propertyCode.substr(rangeMinStart, rangeMinEnd - rangeMinStart);

				auto rangeMaxStart = propertyCode.find_first_not_of(" \t", rangeMinEnd + 1);
				auto rangeMaxEnd = propertyCode.find_first_of(" \t]", rangeMaxStart);
				auto rangeMaxStr = propertyCode.substr(rangeMaxStart, rangeMaxEnd - rangeMaxStart);

				auto rangeMinValue = AX::Tool::StringConverter::ToF32(rangeMinStr);
				auto rangeMaxValue = AX::Tool::StringConverter::ToF32(rangeMaxStr);
				property.MinValue.AsFloat(rangeMinValue);
				property.MaxValue.AsFloat(rangeMaxValue);
			}

			auto propertyAssignment = propertyCode.find_first_of('=', propertyNameEnd);
			if (propertyAssignment < propertyEnd)
			{
				auto defaultValueStart = propertyCode.find_first_not_of(" \t", propertyAssignment + 1);
				auto defaultValueEnd = propertyCode.find_first_of(" \t\n", defaultValueStart);
				auto defaultValueStr = propertyCode.substr(defaultValueStart, defaultValueEnd - defaultValueStart);

				auto defaultValue = AX::Tool::StringConverter::ToF32(defaultValueStr);
				property.DefaultValue.AsFloat(defaultValue);
			}
		}
	}

	auto ShaderHelper::CreateShader(ShaderType type, const ParseShaderSourceResult& parsedShaderSource) -> unsigned int
	{
		auto shaderName = std::string("Vertex");
		auto shaderType = GL_VERTEX_SHADER;
		auto shaderCode = &parsedShaderSource.VertexShaderSource;
		if (type == ShaderType::Fragment)
		{
			shaderName = "Fragment";
			shaderType = GL_FRAGMENT_SHADER;
			shaderCode = &parsedShaderSource.FragmentShaderSource;
		}

		auto shader = glCreateShader(shaderType);
		auto source = shaderCode->c_str();

		glShaderSource(shader, 1, &source, nullptr);
		glCompileShader(shader);
		{
			auto compileStatus = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
			if (compileStatus == GL_FALSE)
			{
				auto infoLogLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

				auto infoLog = std::string(infoLogLength, '\0');
				glGetShaderInfoLog(shader, infoLogLength, nullptr, infoLog.data());

				AX_LogInfo("\n" + GetCodeWithLineNumbers(*shaderCode));
				AX_SafeAssert(false, shaderName + " shader error: " + infoLog);
			}
		}

		return shader;
	}
	auto ShaderHelper::CreateShaderProgram(const ParseShaderSourceResult& parsedShaderSource, unsigned int vertexShader, unsigned int fragmentShader) -> unsigned int
	{
		auto program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		glLinkProgram(program);
		{
			auto linkStatus = 0;
			glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
			if (linkStatus == GL_FALSE)
			{
				auto infoLogLength = 0;
				glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

				auto infoLog = std::string(infoLogLength, '\0');
				glGetProgramInfoLog(program, infoLogLength, nullptr, infoLog.data());

				AX_LogInfo(
					"\nVertex shader source: \n" +
					GetCodeWithLineNumbers(parsedShaderSource.VertexShaderSource) +
					"\nFragment shader source: \n" +
					GetCodeWithLineNumbers(parsedShaderSource.FragmentShaderSource)
				);
				AX_SafeAssert(false, "Shader program link error: " + infoLog);
			}
		}

		glValidateProgram(program);
		{
			auto validateStatus = 0;
			glGetProgramiv(program, GL_VALIDATE_STATUS, &validateStatus);
			if (validateStatus == GL_FALSE)
			{
				auto infoLogLength = 0;
				glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

				auto infoLog = std::string(infoLogLength, '\0');
				glGetProgramInfoLog(program, infoLogLength, nullptr, infoLog.data());

				AX_LogInfo(
					"\nVertex shader source: \n" +
					GetCodeWithLineNumbers(parsedShaderSource.VertexShaderSource) +
					"\nFragment shader source: \n" +
					GetCodeWithLineNumbers(parsedShaderSource.FragmentShaderSource)
				);
				AX_SafeAssert(false, "Shader program validation error: " + infoLog);
			}
		}

		return program;
	}

	auto ShaderHelper::GetCodeWithLineNumbers(const std::string& code) -> std::string
	{
		auto result = std::string();
		auto lineNumber = 1u;
		auto codeCursor = 0ull;
		while (codeCursor < code.size())
		{
			auto lineEnd = code.find_first_of('\n', codeCursor) + 1;
			auto line = code.substr(codeCursor, lineEnd - codeCursor);
			codeCursor = lineEnd;

			result += std::to_string(lineNumber++) + "\t" + line;
		}
		
		return result;
	}

	auto ShaderHelper::GetUniformLocation(Shader* self, const std::string& name) -> int
	{
#ifndef AX_CONFIG_SHIPPING
		if (self->m_UniformLocations.contains(name) == false)
		{
			AX_LogError("Uniform \"" + name + "\" not found in \"" + self->m_Name + "\" shader");
			return -1;
		}
#endif // !AX_CONFIG_SHIPPING

		return self->m_UniformLocations.at(name);
	}
}
