#pragma once
#include <glm/ext/matrix_float4x4.hpp>
#include <string>
#include <vector>
#include "Model.h"
#include "Bone.h"

struct AssimpNodeData {
	glm::mat4 transformation;
	std::string name;
	int childrenCount;
	std::vector<AssimpNodeData> children;
};

class Animation
{
public:
	Animation() = default;
	Animation(const std::string& animationPath, Model* model);
	~Animation();

	Bone* FindBone(const std::string& name);

	const float GetTicksPerSecond() const { return m_TicksPerSecond; }
	const float GetDuration() const { return m_Duration; }
	const AssimpNodeData& GetRootNode() const { return m_RootNode; }
	const std::map<std::string, BoneInfo>& GetBoneInfoMap() const { return m_BoneInfoMap; }

private:
	void ReadMissingBones(const aiAnimation* animation, Model& model);
	void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src);

	float m_Duration;
	int m_TicksPerSecond;
	std::vector<Bone> m_Bones;
	AssimpNodeData m_RootNode;
	std::map<std::string, BoneInfo> m_BoneInfoMap;
};