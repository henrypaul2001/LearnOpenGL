#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "glm/glm.hpp"

class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath);

	void use();

	// uniform utility functions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4(const std::string& name, glm::mat4 value) const;
	void setVec2(const std::string& name, glm::vec2 value) const;
	void setVec2(const std::string& name, float x, float y) const;
	void setVec3(const std::string& name, glm::vec3 value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
	unsigned int GetID() { return ID; }

private:
	unsigned int ID;
};

#endif