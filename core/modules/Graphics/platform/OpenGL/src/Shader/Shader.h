#pragma once


#include "Texture/TextureManager.h"

#define GL_CORE_NULL_SHADER_ID 0 
namespace glCore {
	using ShaderID = unsigned int;

	typedef unsigned int UniformID;
	class Shader {
	private:
		ShaderID m_ID;
		std::unordered_map<std::string, UniformID> m_uniforms;
		int* refCount;
	public:
		Shader();
		Shader(const Shader& other);
		~Shader();

		void LoadShader(const std::string& vertexFilepath, const std::string& fragmentFilepath);
		void LoadDefaultShader();
		void CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

		void Bind() const;
		ShaderID GetID() const;


		template <typename type>
		void SetUniform(std::string uName, type& uValue) {
			printf("Warning: Using template uniform!\n");
		}

		template<>
		void SetUniform<glm::mat4>(std::string uName, glm::mat4& matrix) {
			glUniformMatrix4fv(GetUniform(uName), 1, GL_FALSE, &matrix[0][0]);
		}

		template<>
		void SetUniform<std::array<TextureID, MAX_TEXTURES>>(std::string uName, std::array<TextureID, MAX_TEXTURES>& samplers) {
			glUniform1iv(GetUniform(uName), MAX_TEXTURES, (int*)samplers.data());
		}

		template<>
		void SetUniform<TextureID>(std::string uName, TextureID& sampler) {
			glUniform1i(GetUniform(uName), (int)sampler);
		}

	private:
		UniformID GetUniform(std::string& variableName);
		const std::string LoadFromFile(const std::string& filepath);
		int CompileShader(unsigned int type, const std::string& source);
	};

}