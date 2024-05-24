#include "AnimatorNew.h"

AnimatorNew::AnimatorNew(AnimationNew* animation, ModelNew* animationTarget)
{
	m_CurrentTime = 0.0f;
	m_CurrentAnimation = animation;
	m_AnimationTarget = animationTarget;
	m_DeltaTime = 0.0f;
}

AnimatorNew::~AnimatorNew()
{
}

void AnimatorNew::UpdateAnimation(float dt)
{
	m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * dt;
	m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());

	m_CurrentAnimation->Update(dt, m_CurrentTime, m_AnimationTarget);
}

void AnimatorNew::PlayAnimation(AnimationNew* animation)
{
	m_CurrentAnimation = animation;
	m_CurrentTime = 0.0f;
}