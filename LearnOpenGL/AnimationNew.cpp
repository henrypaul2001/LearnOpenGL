#include "AnimationNew.h"

AnimationNew::AnimationNew(const std::string& animationPath)
{
	ReadChannels(animationPath);
	m_FinalBoneMatrices = std::vector<glm::mat4>(100);
}

AnimationNew::~AnimationNew() {}

void AnimationNew::Update(const float deltaTime, const float currentTime, ModelNew* animationTarget)
{
	CalculateBoneTransformsRecursive(*animationTarget->GetRootBone(), animationTarget->GetSkeletonOriginTransform(), animationTarget, currentTime);
}

AnimationChannel* AnimationNew::FindAnimationChannel(const std::string& channelName)
{
	for (AnimationChannel& channel : m_Channels) {
		if (channel.GetChannelName() == channelName) { return &channel; }
	}
	return nullptr;
}

void AnimationNew::ReadChannels(const std::string& animationPath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
	assert(scene && scene->mRootNode);
	auto animation = scene->mAnimations[0];
	m_Duration = animation->mDuration;
	m_TicksPerSecond = animation->mTicksPerSecond;

	int numChannels = animation->mNumChannels;
	m_Channels.reserve(numChannels);

	for (int i = 0; i < numChannels; i++) {
		aiNodeAnim* aiChannel = animation->mChannels[i];
		
		std::string channelName = aiChannel->mNodeName.C_Str();

		// Channel position keyframes
		std::vector<KeyPositionNew> positions;
		int numPositions = aiChannel->mNumPositionKeys;
		positions.reserve(numPositions);
		for (int j = 0; j < numPositions; j++) {
			aiVector3D aiPosition = aiChannel->mPositionKeys[j].mValue;
			float timeStamp = aiChannel->mPositionKeys[j].mTime;

			KeyPositionNew key;
			key.position = glm::vec3(aiPosition.x, aiPosition.y, aiPosition.z);
			key.timeStamp = timeStamp;
			positions.push_back(key);
		}

		// Channel rotation keyframes
		std::vector<KeyRotationNew> rotations;
		int numRotations = aiChannel->mNumRotationKeys;
		rotations.reserve(numRotations);
		for (int j = 0; j < numRotations; j++) {
			aiQuaternion aiOrientation = aiChannel->mRotationKeys[j].mValue;
			float timeStamp = aiChannel->mRotationKeys[j].mTime;

			KeyRotationNew key;
			key.orientation = glm::quat(aiOrientation.w, aiOrientation.x, aiOrientation.y, aiOrientation.z);
			key.timeStamp = timeStamp;
			rotations.push_back(key);
		}

		// Channel scale keyframes
		std::vector<KeyScaleNew> scalings;
		int numScalings = aiChannel->mNumScalingKeys;
		scalings.reserve(numScalings);
		for (int j = 0; j < numScalings; j++) {
			aiVector3D scale = aiChannel->mScalingKeys[j].mValue;
			float timeStamp = aiChannel->mScalingKeys[j].mTime;

			KeyScaleNew key;
			key.scale = glm::vec3(scale.x, scale.y, scale.z);
			key.timeStamp = timeStamp;
			scalings.push_back(key);
		}

		m_Channels.push_back(AnimationChannel(channelName, i, positions, rotations, scalings));
	}
}

void AnimationNew::CalculateBoneTransformsRecursive(const BoneNew& bone, const glm::mat4& parentTransform, const ModelNew* animationTarget, const float currentTime)
{
	std::string boneName = bone.Name;
	glm::mat4 nodeTransform = bone.nodeTransform;

	AnimationChannel* channel = FindAnimationChannel(boneName);

	if (channel) {
		channel->Update(currentTime);
		nodeTransform = channel->GetProcessedTransform();
	}

	glm::mat4 globalTransformation = parentTransform * nodeTransform;

	int index = bone.BoneID;
	glm::mat4 offset = bone.offsetMatrix;

	m_FinalBoneMatrices[index] = globalTransformation * offset;

	std::map<std::string, BoneNew> bones = animationTarget->GetBones();
	for (const std::string& boneName : bone.childNodeNames) {
		if (bones.find(boneName) != bones.end()) {
			BoneNew& child = bones[boneName];
			CalculateBoneTransformsRecursive(child, globalTransformation, animationTarget, currentTime);
		}
	}
}