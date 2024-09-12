#include "ComputeShader.h"
#include <glm/gtc/type_ptr.hpp>

ComputeShader::ComputeShader(const char* computePath)
{
	// retrieve source code from file
	std::string computeCode;
	std::ifstream cShaderFile;

	// ensure ifstream objects can throw exceptions
	cShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		// open file
		cShaderFile.open(computePath);
		std::stringstream cShaderStream;

		// read files buffer contents into stream
		cShaderStream << cShaderFile.rdbuf();

		// close file
		cShaderFile.close();

		// convert stream into string
		computeCode = cShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::COMPUTESHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* cShaderCode = computeCode.c_str();

	//  compile shader
	unsigned int compute;
	int success;
	char infoLog[512];

	// vertex
	compute = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(compute, 1, &cShaderCode, NULL);
	glCompileShader(compute);

	// print compile errors
	glGetShaderiv(compute, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(compute, 512, NULL, infoLog);
		std::cout << "ERROR::COMPUTESHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	ID = glCreateProgram();
	glAttachShader(ID, compute);
	glLinkProgram(ID);

	// print linking errors
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::COMPUTESHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// delete shader (no longer needed after being linked to program)
	glDeleteShader(compute);
}

ComputeShader::~ComputeShader()
{
}

void ComputeShader::use() const
{
	glUseProgram(ID);
}

void ComputeShader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void ComputeShader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void ComputeShader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void ComputeShader::setMat4(const std::string& name, glm::mat4 value) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void ComputeShader::setMat3(const std::string& name, glm::mat3 value) const
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void ComputeShader::setVec2(const std::string& name, glm::vec2 value) const
{
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void ComputeShader::setVec2(const std::string& name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void ComputeShader::setVec3(const std::string& name, glm::vec3 value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void ComputeShader::setVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void ComputeShader::setVec4(const std::string& name, glm::vec4 value) const
{
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void ComputeShader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}