#pragma once
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "glm/glm.hpp"
class ComputeShader
{
public:
	ComputeShader(const char* computePath);
	~ComputeShader();

	const unsigned int GetID() const { return ID; }
private:
	unsigned int ID;
};