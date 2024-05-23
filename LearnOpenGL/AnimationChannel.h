#pragma once
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <vector>
#include <glm/gtx/quaternion.hpp>
#include <string>

struct KeyPositionNew {
	glm::vec3 position;
	float timeStamp;
};

struct KeyRotationNew {
	glm::quat orientation;
	float timeStamp;
};

struct KeyScaleNew {
	glm::vec3 scale;
	float timeStamp;
};

class AnimationChannel
{
public:
	AnimationChannel(const std::string& name, int ID, std::vector<KeyPositionNew> positions, std::vector<KeyRotationNew> rotations, std::vector<KeyScaleNew> scales);
	~AnimationChannel();

	const glm::mat4& GetProcessedTransform() const { return m_ProcessedTransform; }
	const std::string GetChannelName() const { return m_Name; }
	const int GetChannelID() const { return m_ID; }

	/*interpolates  b/w positions,rotations & scaling keys based on the current time of
	the animation and prepares the local transformation matrix by combining all keys
	tranformations*/
	void Update(float animationTime);

	/* Gets the current index on mKeyPositions to interpolate to based on
	the current animation time*/
	int GetPositionIndex(float animationTime);

	/* Gets the current index on mKeyRotations to interpolate to based on the
	current animation time*/
	int GetRotationIndex(float animationTime);

	/* Gets the current index on mKeyScalings to interpolate to based on the
	current animation time */
	int GetScaleIndex(float animationTime);
private:
	std::vector<KeyPositionNew> m_Positions;
	std::vector<KeyRotationNew> m_Rotations;
	std::vector<KeyScaleNew> m_Scales;
	int m_NumPositions;
	int m_NumRotations;
	int m_NumScalings;

	glm::mat4 m_ProcessedTransform;
	std::string m_Name;
	int m_ID;

	float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);
	glm::mat4 InterpolatePosition(float animationTime);
	glm::mat4 InterpolateRotation(float animationTime);
	glm::mat4 InterpolateScaling(float animationTime);
};