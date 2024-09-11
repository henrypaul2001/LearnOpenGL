#include "ComputeShader.h"

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