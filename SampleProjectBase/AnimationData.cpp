#include "pch.h"
#include "AnimationData.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace DirectX::SimpleMath;

bool AnimationData::HasScaleTwoKeys(u_int _nodeId) const
{
	return pAnimationData->mChannels[_nodeId]->mNumScalingKeys >= 2;
}

bool AnimationData::HasQuatTwoKeys(u_int _nodeId) const
{
	return pAnimationData->mChannels[_nodeId]->mNumRotationKeys >= 2;
}

bool AnimationData::HasPosTwoKeys(u_int _nodeId) const
{
	return pAnimationData->mChannels[_nodeId]->mNumPositionKeys >= 2;
}

void AnimationData::SetAiScene(const aiScene* _pAiScene)
{
	pAiScene = _pAiScene;

	pAnimationData = pAiScene->mAnimations[0];

	// ボーンのサイズを合わせる
	u_int nodeNum = pAnimationData->mNumChannels;
	boneIndicies.resize(nodeNum);

	// アニメーションの全体時間を求める
	CalculateAnimTime();
}

void AnimationData::SetBoneIdx(u_int _nodeId, u_int _boneIdx)
{
	assert(static_cast<u_int>(boneIndicies.size()) < _nodeId);

	boneIndicies[_nodeId] = _boneIdx;
}

std::string AnimationData::GetBoneName(u_int _nodeId)
{
	assert(pAnimationData->mNumChannels > _nodeId && "ノードIDが配列外を指しています");

	return pAnimationData->mChannels[_nodeId]->mNodeName.C_Str();
}

u_int AnimationData::GetChannelCount()
{
	return pAnimationData->mNumChannels;
}

DirectX::SimpleMath::Vector3 AnimationData::GetScale(u_int _nodeId, float _playingTime) const
{
	// 2キー以上なければ補間出来ないので
	if (!HasScaleTwoKeys(_nodeId))
	{
		// 最初のキー値を返す
		return GetScaleByKey(_nodeId, 0);
	}

	Vector3 animScale;

	const aiNodeAnim* pAiNodeAnim = pAnimationData->mChannels[_nodeId];

	// 一つ前・次のスケーリングキー取得
	u_int prevKey = FindPreviousScaleKey(_playingTime, pAiNodeAnim);
	u_int nextKey = prevKey + 1;

	// 最大キーを超えたら最初に戻す
	if (nextKey >= pAiNodeAnim->mNumScalingKeys)
		nextKey = 0;

	// 前のスケール値
	aiVector3D aiPrevScale = pAiNodeAnim->mScalingKeys[prevKey].mValue;

	// 次のスケール値
	aiVector3D aiNextScale = pAiNodeAnim->mScalingKeys[nextKey].mValue;

	animScale.x = aiNextScale.x;
	animScale.y = aiNextScale.y;
	animScale.z = aiNextScale.z;

	return animScale;
}

DirectX::SimpleMath::Quaternion AnimationData::GetQuaternion(u_int _nodeId, float _playingTime) const
{
	// 2キー以上なければ補間出来ないので
	if (!HasQuatTwoKeys(_nodeId))
	{
		// 最初のキー値を返す
		return GetQuatByKey(_nodeId, 0);
	}

	Quaternion animQuat;

	const aiNodeAnim* pAiNodeAnim = pAnimationData->mChannels[_nodeId];

	// 一つ前・次の回転キー取得
	u_int prevKey = FindPreviousRotKey(_playingTime, pAiNodeAnim);
	u_int nextKey = prevKey + 1;

	// 最大キーを超えたら最初に戻す
	if (nextKey >= pAiNodeAnim->mNumRotationKeys)
		nextKey = 0;

	// 前のクォータニオン
	aiQuaternion aiQuat = pAiNodeAnim->mRotationKeys[prevKey].mValue;

	// 次のクォータニオン
	aiQuat = pAiNodeAnim->mRotationKeys[nextKey].mValue;
	animQuat.x = aiQuat.x;
	animQuat.y = aiQuat.y;
	animQuat.z = aiQuat.z;
	animQuat.w = aiQuat.w;

	// 正規化
	animQuat.Normalize();

	return animQuat;
}

DirectX::SimpleMath::Vector3 AnimationData::GetPosition(u_int _nodeId, float _playingTime) const
{
	// 2キー以上なければ補間出来ないので
	if (!HasPosTwoKeys(_nodeId))
	{
		// 最初のキー値を返す
		return GetPosByKey(_nodeId, 0);
	}

	Vector3 animPos;

	const aiNodeAnim* pAiNodeAnim = pAnimationData->mChannels[_nodeId];

	// 一つ前・次のスケーリングキー取得
	u_int prevKey = FindPreviousPosKey(_playingTime, pAiNodeAnim);
	u_int nextKey = prevKey + 1;

	// 最大キーを超えたら最初に戻す
	if (nextKey >= pAiNodeAnim->mNumPositionKeys)
		nextKey = 0;

	// 前のスケール値
	aiVector3D aiPrevPos = pAiNodeAnim->mPositionKeys[prevKey].mValue;

	// 次のスケール値
	aiVector3D aiNextPos = pAiNodeAnim->mPositionKeys[nextKey].mValue;

	animPos.x = aiNextPos.x;
	animPos.y = aiNextPos.y;
	animPos.z = aiNextPos.z;

	return animPos;
}

float AnimationData::GetAnimationTime() const
{
	return animationTime;
}

const aiNode* AnimationData::GetRootNode()
{
	return pAiScene->mRootNode;
}

const aiNodeAnim* AnimationData::GetAiNodeAnim(const std::string& _nodeName)
{
	for (u_int c_i = 0; c_i < pAnimationData->mNumChannels; c_i++)
	{
		if (pAnimationData->mChannels[c_i]->mNodeName.C_Str() == _nodeName)
		{
			return pAnimationData->mChannels[c_i];
		}
	}

	assert(!"アニメーションノードが見つかりませんでした");
	return nullptr;
}

void AnimationData::CalculateAnimTime()
{
	// アニメーションの全体ティック数
	double durationTicks = pAnimationData->mDuration;

	// 1秒間のティック数を求める
	// mTicksPerSecondが0なら1秒間のティックは1
	double ticksPerSecond =
		pAnimationData->mTicksPerSecond != 0 ? pAnimationData->mTicksPerSecond : 1.0f;

	// 時間を求める
	double time = durationTicks / ticksPerSecond;

	animationTime = static_cast<float>(time);
}

u_int AnimationData::FindPreviousRotKey(float _playingTime, const aiNodeAnim* _pAiNodeAnim) const
{
	assert(_pAiNodeAnim->mNumRotationKeys > 0 && "回転キーありません");

	// 時間から以前のキーを取得
	for (u_int k_i = 0; k_i < _pAiNodeAnim->mNumRotationKeys - 1; k_i++)
	{
		if (_playingTime < _pAiNodeAnim->mRotationKeys[k_i + 1].mTime)
			return k_i;
	}

	/*assert(!"回転キーが正常に取得できませんでした");*/
	std::string message = std::string(_pAiNodeAnim->mNodeName.C_Str());
	message += " 回転キーが正常に取得できませんでした";
	HASHI_DEBUG_LOG(message);
	return 0;
}

u_int AnimationData::FindPreviousScaleKey(float _playingTime, const aiNodeAnim* _pAiNodeAnim) const
{
	assert(_pAiNodeAnim->mNumScalingKeys > 0 && "スケーリングキーありません");

	// 時間から以前のキーを取得
	for (u_int k_i = 0; k_i < _pAiNodeAnim->mNumScalingKeys - 1; k_i++)
	{
		if (_playingTime < _pAiNodeAnim->mScalingKeys[k_i + 1].mTime)
			return k_i;
	}
	std::string message = std::string(_pAiNodeAnim->mNodeName.C_Str());
	message += " スケーリングキーが正常に取得できませんでした";
	/*assert(!message.c_str());*/
	HASHI_DEBUG_LOG(message);
	return 0;
}

u_int AnimationData::FindPreviousPosKey(float _playingTime, const aiNodeAnim* _pAiNodeAnim) const
{
	assert(_pAiNodeAnim->mNumPositionKeys > 0 && "座標キーありません");

	// 時間から以前のキーを取得
	for (u_int k_i = 0; k_i < _pAiNodeAnim->mNumPositionKeys - 1; k_i++)
	{
		if (_playingTime < _pAiNodeAnim->mPositionKeys[k_i + 1].mTime)
			return k_i;
	}

	assert(!"座標キーが正常に取得できませんでした");
	return 0;
}

DirectX::SimpleMath::Vector3 AnimationData::GetScaleByKey(u_int _nodeId, u_int _key) const
{
	aiNodeAnim* aiNodeAnim = pAnimationData->mChannels[_nodeId];

	// キー数より多い数字がきたら
	assert(aiNodeAnim->mNumScalingKeys > _key);

	aiVector3D aiScale = aiNodeAnim->mScalingKeys[_key].mValue;

	Vector3 scale;
	scale.x = aiScale.x;
	scale.y = aiScale.y;
	scale.z = aiScale.z;

	return scale;
}

DirectX::SimpleMath::Quaternion AnimationData::GetQuatByKey(u_int _nodeId, u_int _key) const
{
	aiNodeAnim* aiNodeAnim = pAnimationData->mChannels[_nodeId];

	// キー数より多い数字がきたら
	assert(aiNodeAnim->mNumRotationKeys > _key);

	aiQuaternion aiQuat = aiNodeAnim->mRotationKeys[_key].mValue;

	Quaternion quat;
	quat.x = aiQuat.x;
	quat.y = aiQuat.y;
	quat.z = aiQuat.z;
	quat.w = aiQuat.w;

	return quat;
}

DirectX::SimpleMath::Vector3 AnimationData::GetPosByKey(u_int _nodeId, u_int _key) const
{
	aiNodeAnim* aiNodeAnim = pAnimationData->mChannels[_nodeId];

	// キー数より多い数字がきたら
	assert(aiNodeAnim->mNumPositionKeys > _key);

	aiVector3D aiPos = aiNodeAnim->mPositionKeys[_key].mValue;

	Vector3 pos;
	pos.x = aiPos.x;
	pos.y = aiPos.y;
	pos.z = aiPos.z;

	return pos;
}