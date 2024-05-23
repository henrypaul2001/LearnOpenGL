#include "AnimationChannel.h"

AnimationChannel::AnimationChannel(const std::string& name, int ID, std::vector<KeyPositionNew> positions, std::vector<KeyRotationNew> rotations, std::vector<KeyScaleNew> scales)
{
	m_Name = name;
	m_ID = ID;
	m_Positions = positions;
	m_Rotations = rotations;
	m_Scales = scales;
	m_NumPositions = positions.size();
	m_NumRotations = rotations.size();
	m_NumScalings = scales.size();
	m_ProcessedTransform = glm::mat4(1.0f);
}

AnimationChannel::~AnimationChannel() {}

void AnimationChannel::Update(float animationTime)
{
	glm::mat4 translation = InterpolatePosition(animationTime);
	glm::mat4 rotation = InterpolateRotation(animationTime);
	glm::mat4 scale = InterpolateScaling(animationTime);
	m_ProcessedTransform = translation * rotation * scale;
}

int AnimationChannel::GetPositionIndex(float animationTime)
{
	for (int index = 0; index < m_NumPositions - 1; ++index) {
		if (animationTime < m_Positions[index + 1].timeStamp) { return index; }
	}
	assert(0);
}

int AnimationChannel::GetRotationIndex(float animationTime)
{
	for (int index = 0; index < m_NumRotations - 1; ++index) {
		if (animationTime < m_Rotations[index + 1].timeStamp) { return index; }
	}
	assert(0);
}

int AnimationChannel::GetScaleIndex(float animationTime)
{
	for (int index = 0; index < m_NumScalings - 1; ++index) {
		if (animationTime < m_Scales[index + 1].timeStamp) { return index; }
	}
	assert(0);
}

float AnimationChannel::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
{
	float scaleFactor = 0.0f;
	float midWayLength = animationTime - lastTimeStamp;
	float framesDiff = nextTimeStamp - lastTimeStamp;
	scaleFactor = midWayLength / framesDiff;
	return scaleFactor;
}

glm::mat4 AnimationChannel::InterpolatePosition(float animationTime)
{
	if (m_NumPositions == 1) {
		return glm::translate(glm::mat4(1.0f), m_Positions[0].position);
	}

	int p0Index = GetPositionIndex(animationTime);
	int p1Index = p0Index + 1;

	float scaleFactor = GetScaleFactor(m_Positions[p0Index].timeStamp, m_Positions[p1Index].timeStamp, animationTime);
	glm::vec3 finalPosition = glm::mix(m_Positions[p0Index].position, m_Positions[p1Index].position, scaleFactor);

	return glm::translate(glm::mat4(1.0f), finalPosition);
}

glm::mat4 AnimationChannel::InterpolateRotation(float animationTime)
{
	if (m_NumRotations == 1) {
		glm::quat rotation = glm::normalize(m_Rotations[0].orientation);
		return glm::toMat4(rotation);
	}

	int p0Index = GetRotationIndex(animationTime);
	int p1Index = p0Index + 1;

	float scaleFactor = GetScaleFactor(m_Rotations[p0Index].timeStamp, m_Rotations[p1Index].timeStamp, animationTime);
	glm::quat finalRotation = glm::slerp(m_Rotations[p0Index].orientation, m_Rotations[p1Index].orientation, scaleFactor);

	return glm::toMat4(finalRotation);
}

glm::mat4 AnimationChannel::InterpolateScaling(float animationTime)
{
	if (m_NumScalings == 1) {
		return glm::scale(glm::mat4(1.0f), m_Scales[0].scale);
	}

	int p0Index = GetScaleIndex(animationTime);
	int p1Index = p0Index + 1;

	float scaleFactor = GetScaleFactor(m_Scales[p0Index].timeStamp, m_Scales[p1Index].timeStamp, animationTime);
	glm::vec3 finalScale = glm::mix(m_Scales[p0Index].scale, m_Scales[p1Index].scale, scaleFactor);

	return glm::scale(glm::mat4(1.0f), finalScale);
}
