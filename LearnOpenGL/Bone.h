#pragma once
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <assimp/anim.h>
#include <vector>
struct KeyPosition {
	glm::vec3 position;
	float timeStamp;
};

struct KeyRotation {
	glm::quat orientation;
	float timeStamp;
};

struct KeyScale {
	glm::vec3 scale;
	float timeStamp;
};

class Bone
{
public:
	Bone(const std::string& name, int ID, const aiNodeAnim* channel);
	~Bone();

	const glm::mat4& GetLocalTransform() { return m_LocalTransform; }
	const std::string GetBoneName() const { return m_Name; }
	const int GetBoneID() const { return m_ID; }

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
	std::vector<KeyPosition> m_Positions;
	std::vector<KeyRotation> m_Rotations;
	std::vector<KeyScale> m_Scales;
	int m_NumPositions;
	int m_NumRotations;
	int m_NumScalings;

	glm::mat4 m_LocalTransform;
	std::string m_Name;
	int m_ID;

	float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);
	glm::mat4 InterpolatePosition(float animationTime);
	glm::mat4 InterpolateRotation(float animationTime);
	glm::mat4 InterpolateScaling(float animationTime);
};