#pragma once
#include "Animation.h"
class Animator
{
public:
	Animator(Animation* animation);

	void UpdateAnimation(float dt);
	void PlayAnimation(Animation* animation);
	void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);

	const std::vector<glm::mat4>& GetFinalBoneMatrices() const { return m_FinalBoneMatrices; }

private:
	std::vector<glm::mat4> m_FinalBoneMatrices;
	Animation* m_CurrentAnimation;
	float m_CurrentTime;
	float m_DeltaTime;
};