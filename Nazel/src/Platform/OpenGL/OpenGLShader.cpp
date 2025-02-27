#include "pch.h"
#include "OpenGLShader.h"
#include <fstream>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Nazel {
static GLenum ShaderTypeFromString(const std::string& type) {
	if (type == "vertex") return GL_VERTEX_SHADER;
	
	if (type == "fragment" || type == "pixel")
		return GL_FRAGMENT_SHADER;

	NZ_CORE_ASSERT(false, "Unknown shader type!");
	return 0;
}
OpenGLShader::OpenGLShader(const std::string& filepath) : m_RendererID(0) {
	std::string source = ReadFile(filepath);
	auto shadersources = PreProcess(source);
	Compile(shadersources);

	auto lastSlash = filepath.find_last_of("/\\");
	lastSlash = (std::string::npos == lastSlash ? 0 : lastSlash + 1);
	auto lastDot = filepath.rfind('.');
	auto count = (lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash);
	m_Name = filepath.substr(lastSlash, count);
}
OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) : m_RendererID(0), m_Name(name) {
	std::unordered_map<GLenum, std::string> sources;
	sources[GL_VERTEX_SHADER] = vertexSrc;
	sources[GL_FRAGMENT_SHADER] = fragmentSrc;
	Compile(sources);
}
OpenGLShader::~OpenGLShader() {
	glDeleteProgram(m_RendererID);
}
void OpenGLShader::Bind() const {
	glUseProgram(m_RendererID);
}
void OpenGLShader::Unbind() const {
	glUseProgram(0);
}
const std::string& OpenGLShader::GetName() const {
	return m_Name;
}
void OpenGLShader::UploadUniformInt(const std::string& name, int value) {
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform1i(location, value);
}
void OpenGLShader::UploadUniformFloat(const std::string& name, float value) {
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform1f(location, value);
}
void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value) {
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform2f(location, value.x, value.y);
}
void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value) {
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform3f(location, value.x, value.y, value.z);
}
void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value) {
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform4f(location, value.x, value.y, value.z, value.w);
}
void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix) {
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}
void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) {
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}
std::string OpenGLShader::ReadFile(const std::string& filepath) {
	std::string result;
	std::ifstream in(filepath, std::ios::in | std::ios::binary);
	if (in) {
		in.seekg(0, std::ios::end);
		result.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&result[0], result.size());
		in.close();
	}
	else {
		LOG_CORE_ERROR("Could not open file '{0}'", filepath);
	}
	return result;
}
std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source) {
	std::unordered_map<GLenum, std::string> shaderSources;

	const char* typeToken = "#type";
	size_t typeTokenLength = strlen(typeToken);
	size_t pos = source.find(typeToken, 0);
	while (pos != std::string::npos) {
		// ��ĩβ
		size_t eol = source.find_first_of("\r\n", pos);
		NZ_CORE_ASSERT((eol != std::string::npos), "Syntax error");

		size_t begin = pos + typeTokenLength + 1;
		std::string type = source.substr(begin, eol - begin);
		NZ_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

		size_t nextlinepos = source.find_first_not_of("\r\n", eol);
		pos = source.find(typeToken, nextlinepos);
		shaderSources[ShaderTypeFromString(type)] = source.substr(nextlinepos, pos - (nextlinepos == std::string::npos ? source.size() - 1 : nextlinepos));
	}
	return shaderSources;
}
void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources) {
	GLuint program = glCreateProgram();
	NZ_CORE_ASSERT(shaderSources.size() <= 2, "We only support 2 shaders for now");
	std::array<GLenum, 2> glShaderIDs;
	auto glShaderIDIndex = 0;
	for (auto& kv : shaderSources) {
		auto type = kv.first;
		const auto& source = kv.second;
		GLuint shader = glCreateShader(type);
		
		const char* sourceCStr = source.c_str();
		glShaderSource(shader, 1, &sourceCStr, 0);
		// Compile the fragment shader
		glCompileShader(shader);

		GLint isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
			// We don't need the shader anymore.
			glDeleteShader(shader);
			// Either of them. Don't leak shaders.
			glDeleteShader(shader);
			LOG_CORE_ERROR("{0}", infoLog.data());
			NZ_CORE_ASSERT(false, "Fragment shader compilation failure!");
			break;
		}
		glAttachShader(program, shader);
		glShaderIDs[glShaderIDIndex++] = shader;
	}

	m_RendererID = program;
	glLinkProgram(program);
	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
		// We don't need the program anymore.
		glDeleteProgram(program);
		// Don't leak shaders either.
		for (auto id : glShaderIDs) {
			glDeleteShader(id);
		}
		LOG_CORE_ERROR("{0}", infoLog.data());
		NZ_CORE_ASSERT(false, "Shader link failure!");
		return;
	}
	for (auto id : glShaderIDs) {
		glDeleteShader(id);
	}
}
}

