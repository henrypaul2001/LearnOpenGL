#include "Bone.h"

Bone::Bone(const std::string& name, int ID, const aiNodeAnim* channel) : m_Name(name), m_ID(ID), m_LocalTransform(1.0f)
{
}

Bone::~Bone() {}

void Bone::Update(float animationTime)
{
}

int Bone::GetPositionIndex(float animationTime)
{
	return 0;
}

int Bone::GetRotationIndex(float animationTime)
{
	return 0;
}

int Bone::GetScaleIndex(float animationTime)
{
	return 0;
}

float Bone::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
{
	return 0.0f;
}

glm::mat4 Bone::InterpolatePosition(float animationTime)
{
	return glm::mat4();
}

glm::mat4 Bone::InterpolateRotation(float animationTime)
{
	return glm::mat4();
}

glm::mat4 Bone::InterpolateScaling(float animationTime)
{
	return glm::mat4();
}
