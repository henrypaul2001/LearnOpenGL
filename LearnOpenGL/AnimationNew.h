#pragma once
#include <vector>
#include <glm/ext/matrix_float4x4.hpp>
#include <string>
#include "ModelNew.h"
#include "AnimationChannel.h"
class AnimationNew
{
public:
	AnimationNew() = default;
	AnimationNew(const std::string& animationPath, int animationIndex);
	~AnimationNew();

	void Update(const float deltaTime, const float currentTime, ModelNew* animationTarget);

	const AnimationChannel* GetAnimationChannelAtIndex(const int index) const { return &m_Channels[index]; }
	AnimationChannel* FindAnimationChannel(const std::string& channelName);
	//Bone* FindBone(const std::string& name);

	const std::vector<glm::mat4>& GetFinalBoneMatrices() const { return m_FinalBoneMatrices; }

	const float GetTicksPerSecond() const { return m_TicksPerSecond; }
	const float GetDuration() const { return m_Duration; }
	//const ModelNew* GetAnimationTarget() const { return m_AnimationTarget; }
	//const AssimpNodeData& GetRootNode() const { return m_RootNode; }
	//const std::map<std::string, BoneInfo>& GetBoneInfoMap() const { return m_BoneInfoMap; }

private:
	//void ReadMissingBones(const aiAnimation* animation, Model& model);
	//void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src);

	void CalculateBoneTransformsRecursive(const BoneNew& bone, const glm::mat4& parentTransform, const ModelNew* animationTarget, const float currentTime);

	void ReadChannels(const std::string& animationPath, int animationIndex);

	float m_Duration;
	int m_TicksPerSecond;
	std::vector<AnimationChannel> m_Channels;
	//ModelNew* m_AnimationTarget;

	std::vector<glm::mat4> m_FinalBoneMatrices;
	//AssimpNodeData m_RootNode;
	//std::map<std::string, BoneInfo> m_BoneInfoMap;
};