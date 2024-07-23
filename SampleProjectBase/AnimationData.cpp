#include "pch.h"
#include "AnimationData.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void AnimationData::SetAiScene(const aiScene* _pAiScene)
{
	pAiScene = _pAiScene;

	pAnimationData = pAiScene->mAnimations[0];

	// ボーンのサイズを合わせる
	u_int nodeNum = pAnimationData->mNumChannels;
	boneIndicies.resize(nodeNum);
}

void AnimationData::SetBoneIdx(u_int _nodeId, u_int _boneIdx)
{
	assert(static_cast<u_int>(boneIndicies.size()) < _nodeId);

	boneIndicies[_nodeId] = _boneIdx;
}

std::string AnimationData::GetBoneName(u_int _nodeId)
{
	assert(pAnimationData->mNumChannels > _nodeId);

	return pAnimationData->mChannels[_nodeId]->mNodeName.C_Str();
}

u_int AnimationData::GetChannelCount()
{
	return pAnimationData->mNumChannels;
}
