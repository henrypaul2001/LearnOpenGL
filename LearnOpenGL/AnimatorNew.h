#pragma once
#include "AnimationNew.h"
class AnimatorNew
{
public:
	AnimatorNew(AnimationNew* animation, ModelNew* animationTarget);
	~AnimatorNew();

	const std::vector<glm::mat4>& GetFinalBoneTransforms() const { return m_CurrentAnimation->GetFinalBoneMatrices(); }

	void UpdateAnimation(float dt);
	void PlayAnimation(AnimationNew* animation);
	//void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);

private:
	bool paused;
	ModelNew* m_AnimationTarget;
	AnimationNew* m_CurrentAnimation;
	float m_CurrentTime;
	float m_DeltaTime;
};