#include "Shader.h"

#include "glad/glad.h"
#include "stb/stb_image.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace Engine
{
	std::vector<Shader *> Shader::p_ShaderList;

	Shader::Shader(const char *vertShaderSource /*=NULL*/,
				   const char *fragShaderSource /*=NULL*/
	)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (vertShaderSource == nullptr || fragShaderSource == nullptr)
			return;

		m_ShaderProgramID = glCreateProgram();

		unsigned int vertShaderID = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertShaderID, 1, &vertShaderSource, nullptr);
		glCompileShader(vertShaderID);
		if (!CheckErrors(vertShaderID))
			glAttachShader(m_ShaderProgramID, vertShaderID);

		unsigned int fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragShaderID, 1, &fragShaderSource, nullptr);
		glCompileShader(fragShaderID);
		if (!CheckErrors(fragShaderID))
			glAttachShader(m_ShaderProgramID, fragShaderID);

		glLinkProgram(m_ShaderProgramID);

		int success = false;
		// check for linking errors
		int infoLogLen;
		glGetProgramiv(m_ShaderProgramID, GL_INFO_LOG_LENGTH, &infoLogLen);
		glGetProgramiv(m_ShaderProgramID, GL_LINK_STATUS, &success);
		if (!success) {
			if (infoLogLen > 0) {
				std::vector<char> errorLog(1024);
				glGetProgramInfoLog(
					m_ShaderProgramID, 1024, nullptr, &errorLog[0]);

				std::stringstream logStream;
				for (char character : errorLog) {
					logStream << character;
				}

				std::cout << logStream.str();
			} else {
				std::cout << "SHADER PROGRAM ERROR WITH NO INFO" << std::endl;
			}
		}

		glDetachShader(m_ShaderProgramID, vertShaderID);
		glDetachShader(m_ShaderProgramID, fragShaderID);
		glDeleteShader(vertShaderID);
		glDeleteShader(fragShaderID);


		m_ShaderIndex = p_ShaderList.size();
		p_ShaderList.push_back(this);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_ShaderProgramID);
		p_ShaderList.erase(p_ShaderList.begin() + m_ShaderIndex);
	}

	Shader Shader::Compile(const char *vertPath, const char *fragPath)
	{
		std::string vertShaderSource = ReadFile(vertPath);
		std::string fragShaderSource = ReadFile(fragPath);

		return {vertShaderSource.c_str(), fragShaderSource.c_str()};
	}

#pragma region SetUniformFuncs

	void Shader::SetUniform(int loc, int value) const
	{
		Bind();
		glUniform1i(loc, value);
		Unbind();
	}

	void Shader::SetUniform(int loc, bool value) const
	{
		Bind();
		glUniform1i(loc, value);
		Unbind();
	}

	void Shader::SetUniform(int loc, float value) const
	{
		Bind();
		glUniform1f(loc, value);
		Unbind();
	}

	void Shader::SetUniform(const char *name, int value) const
	{
		Bind();
		glUniform1i(GetUniformLocation(name), value);
		Unbind();
	}

	void Shader::SetUniform(const char *name, bool value) const
	{
		Bind();
		glUniform1i(GetUniformLocation(name), value);
		Unbind();
	}

	void Shader::SetUniform(const char *name, float value) const
	{
		Bind();
		glUniform1f(GetUniformLocation(name), value);
		Unbind();
	}

	void Shader::SetUniformVec(int loc, glm::vec2 value) const
	{
		Bind();
		glUniform2f(loc, value.x, value.y);
		Unbind();
	}
	void Shader::SetUniformVec(int loc, glm::vec3 value) const
	{
		Bind();
		glUniform3f(loc, value.x, value.y, value.z);
		Unbind();
	}
	void Shader::SetUniformVec(int loc, glm::vec4 value) const
	{
		Bind();
		glUniform4f(loc, value.x, value.y, value.z, value.w);
		Unbind();
	}

	void Shader::SetUniformVec(const char *name, glm::vec2 value) const
	{
		int loc = GetUniformLocation(name);
		SetUniformVec(loc, value);
	}
	void Shader::SetUniformVec(const char *name, glm::vec3 value) const
	{
		int loc = GetUniformLocation(name);
		SetUniformVec(loc, value);
	}
	void Shader::SetUniformVec(const char *name, glm::vec4 value) const
	{
		int loc = GetUniformLocation(name);
		SetUniformVec(loc, value);
	}

	void Shader::SetUniformMat4(int loc, glm::mat4 value) const
	{
		Bind();

		glUniformMatrix4fv(loc, 1, false, &value[0][0]);

		Unbind();
	}

	void Shader::SetUniformMat4(const char *name, glm::mat4 value) const
	{
		int loc = GetUniformLocation(name);
		SetUniformMat4(loc, value);
	}

#pragma endregion

	int Shader::GetUniformLocation(const char *name) const
	{ return glGetUniformLocation(m_ShaderProgramID, name); }

	int Shader::GetAttribLocation(const char *name) const
	{ return glGetAttribLocation(m_ShaderProgramID, name); }

	void Shader::Bind() const
	{
		for (const Texture &tex : m_Textures) {
			tex.Bind();
		}
		glUseProgram(m_ShaderProgramID);
	}

	void Shader::Unbind() const
	{
		for (const Texture &tex : m_Textures) {
			tex.Unbind();
		}
		glUseProgram(0);
	}

	void Shader::AttachTexture(Texture tex) { m_Textures.push_back(tex); }

	void Shader::Destroy()
	{
		glDeleteProgram(m_ShaderProgramID);
		for (Texture &texture : m_Textures) {
			texture.Destroy();
		}
	}

	std::string Shader::ReadFile(const char *path)
	{
		std::ifstream file;
		file.open(path);


		std::string contents;

		if (!file.is_open()) {
			std::cout << "ERROR OPENING FILE: " << path << std::endl;
			return "";
		}

		std::string line;
		while (std::getline(file, line)) {
			contents.append(line + "\n");
		}

		file.close();

		return contents;
	}

	bool Shader::CheckErrors(unsigned int shader)
	{
		int isCompiled = false;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (!isCompiled) {
			int maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<char> errorLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

			std::stringstream logStream;

			for (char character : errorLog) {
				logStream << character;
			}

			std::cout << logStream.str();

			// Provide the info log in whatever manner you deem best.
			// Exit with failure.
			glDeleteShader(shader);	   // Don't leak the shader.
		}

		return !isCompiled;
	}

	// Texture
	// ////////////////////////////////////////////////////////////////////

	Texture::Texture(const char *path) { InitFromPath(path); }
	Texture::Texture(int width,
					 int height,
					 int channel,
					 const unsigned char *buffer)
	{ InitFromBuffer(width, height, channel, buffer); }

	void Texture::Bind() const { glBindTexture(GL_TEXTURE_2D, m_TextureID); }

	void Texture::Unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }

	void Texture::Destroy() { glDeleteTextures(1, &m_TextureID); }

	void Texture::InitFromPath(const char *path)
	{
		stbi_set_flip_vertically_on_load(true);

		int width, height, channels = 4;
		unsigned char *image =
			stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);

		if (!image) {
			std::cout << "ERROR: Image not opened" << std::endl;
		}

		InitFromBuffer(width, height, channels, image);

		stbi_image_free(image);
	}

	void Texture::InitFromBuffer(int width,
								 int height,
								 int channel,
								 const unsigned char *buffer)
	{
		glGenTextures(1, &m_TextureID);
		Bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D,
					 0,
					 GL_RGBA,
					 width,
					 height,
					 0,
					 GL_RGBA,
					 GL_UNSIGNED_BYTE,
					 buffer);
		Unbind();
	}

}	 // namespace Engine