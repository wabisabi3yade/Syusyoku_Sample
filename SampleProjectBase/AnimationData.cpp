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

	// �{�[���̃T�C�Y�����킹��
	u_int nodeNum = pAnimationData->mNumChannels;
	boneIndicies.resize(nodeNum);

	// �A�j���[�V�����̑S�̎��Ԃ����߂�
	CalculateAnimTime();
}

void AnimationData::SetBoneIdx(u_int _nodeId, u_int _boneIdx)
{
	assert(static_cast<u_int>(boneIndicies.size()) < _nodeId);

	boneIndicies[_nodeId] = _boneIdx;
}

std::string AnimationData::GetBoneName(u_int _nodeId)
{
	assert(pAnimationData->mNumChannels > _nodeId && "�m�[�hID���z��O���w���Ă��܂�");

	return pAnimationData->mChannels[_nodeId]->mNodeName.C_Str();
}

u_int AnimationData::GetChannelCount()
{
	return pAnimationData->mNumChannels;
}

DirectX::SimpleMath::Vector3 AnimationData::GetScale(u_int _nodeId, float _playingTime) const
{
	// 2�L�[�ȏ�Ȃ���Ε�ԏo���Ȃ��̂�
	if (!HasScaleTwoKeys(_nodeId))
	{
		// �ŏ��̃L�[�l��Ԃ�
		return GetScaleByKey(_nodeId, 0);
	}

	Vector3 animScale;

	const aiNodeAnim* pAiNodeAnim = pAnimationData->mChannels[_nodeId];

	// ��O�E���̃X�P�[�����O�L�[�擾
	u_int prevKey = FindPreviousScaleKey(_playingTime, pAiNodeAnim);
	u_int nextKey = prevKey + 1;

	// �ő�L�[�𒴂�����ŏ��ɖ߂�
	if (nextKey >= pAiNodeAnim->mNumScalingKeys)
		nextKey = 0;

	// �O�̃X�P�[���l
	aiVector3D aiPrevScale = pAiNodeAnim->mScalingKeys[prevKey].mValue;

	// ���̃X�P�[���l
	aiVector3D aiNextScale = pAiNodeAnim->mScalingKeys[nextKey].mValue;

	animScale.x = aiNextScale.x;
	animScale.y = aiNextScale.y;
	animScale.z = aiNextScale.z;

	return animScale;
}

DirectX::SimpleMath::Quaternion AnimationData::GetQuaternion(u_int _nodeId, float _playingTime) const
{
	// 2�L�[�ȏ�Ȃ���Ε�ԏo���Ȃ��̂�
	if (!HasQuatTwoKeys(_nodeId))
	{
		// �ŏ��̃L�[�l��Ԃ�
		return GetQuatByKey(_nodeId, 0);
	}

	Quaternion animQuat;

	const aiNodeAnim* pAiNodeAnim = pAnimationData->mChannels[_nodeId];

	// ��O�E���̉�]�L�[�擾
	u_int prevKey = FindPreviousRotKey(_playingTime, pAiNodeAnim);
	u_int nextKey = prevKey + 1;

	// �ő�L�[�𒴂�����ŏ��ɖ߂�
	if (nextKey >= pAiNodeAnim->mNumRotationKeys)
		nextKey = 0;

	// �O�̃N�H�[�^�j�I��
	aiQuaternion aiQuat = pAiNodeAnim->mRotationKeys[prevKey].mValue;

	// ���̃N�H�[�^�j�I��
	aiQuat = pAiNodeAnim->mRotationKeys[nextKey].mValue;
	animQuat.x = aiQuat.x;
	animQuat.y = aiQuat.y;
	animQuat.z = aiQuat.z;
	animQuat.w = aiQuat.w;

	// ���K��
	animQuat.Normalize();

	return animQuat;
}

DirectX::SimpleMath::Vector3 AnimationData::GetPosition(u_int _nodeId, float _playingTime) const
{
	// 2�L�[�ȏ�Ȃ���Ε�ԏo���Ȃ��̂�
	if (!HasPosTwoKeys(_nodeId))
	{
		// �ŏ��̃L�[�l��Ԃ�
		return GetPosByKey(_nodeId, 0);
	}

	Vector3 animPos;

	const aiNodeAnim* pAiNodeAnim = pAnimationData->mChannels[_nodeId];

	// ��O�E���̃X�P�[�����O�L�[�擾
	u_int prevKey = FindPreviousPosKey(_playingTime, pAiNodeAnim);
	u_int nextKey = prevKey + 1;

	// �ő�L�[�𒴂�����ŏ��ɖ߂�
	if (nextKey >= pAiNodeAnim->mNumPositionKeys)
		nextKey = 0;

	// �O�̃X�P�[���l
	aiVector3D aiPrevPos = pAiNodeAnim->mPositionKeys[prevKey].mValue;

	// ���̃X�P�[���l
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

	assert(!"�A�j���[�V�����m�[�h��������܂���ł���");
	return nullptr;
}

void AnimationData::CalculateAnimTime()
{
	// �A�j���[�V�����̑S�̃e�B�b�N��
	double durationTicks = pAnimationData->mDuration;

	// 1�b�Ԃ̃e�B�b�N�������߂�
	// mTicksPerSecond��0�Ȃ�1�b�Ԃ̃e�B�b�N��1
	double ticksPerSecond =
		pAnimationData->mTicksPerSecond != 0 ? pAnimationData->mTicksPerSecond : 1.0f;

	// ���Ԃ����߂�
	double time = durationTicks / ticksPerSecond;

	animationTime = static_cast<float>(time);
}

u_int AnimationData::FindPreviousRotKey(float _playingTime, const aiNodeAnim* _pAiNodeAnim) const
{
	assert(_pAiNodeAnim->mNumRotationKeys > 0 && "��]�L�[����܂���");

	// ���Ԃ���ȑO�̃L�[���擾
	for (u_int k_i = 0; k_i < _pAiNodeAnim->mNumRotationKeys - 1; k_i++)
	{
		if (_playingTime < _pAiNodeAnim->mRotationKeys[k_i + 1].mTime)
			return k_i;
	}

	/*assert(!"��]�L�[������Ɏ擾�ł��܂���ł���");*/
	std::string message = std::string(_pAiNodeAnim->mNodeName.C_Str());
	message += " ��]�L�[������Ɏ擾�ł��܂���ł���";
	HASHI_DEBUG_LOG(message);
	return 0;
}

u_int AnimationData::FindPreviousScaleKey(float _playingTime, const aiNodeAnim* _pAiNodeAnim) const
{
	assert(_pAiNodeAnim->mNumScalingKeys > 0 && "�X�P�[�����O�L�[����܂���");

	// ���Ԃ���ȑO�̃L�[���擾
	for (u_int k_i = 0; k_i < _pAiNodeAnim->mNumScalingKeys - 1; k_i++)
	{
		if (_playingTime < _pAiNodeAnim->mScalingKeys[k_i + 1].mTime)
			return k_i;
	}
	std::string message = std::string(_pAiNodeAnim->mNodeName.C_Str());
	message += " �X�P�[�����O�L�[������Ɏ擾�ł��܂���ł���";
	/*assert(!message.c_str());*/
	HASHI_DEBUG_LOG(message);
	return 0;
}

u_int AnimationData::FindPreviousPosKey(float _playingTime, const aiNodeAnim* _pAiNodeAnim) const
{
	assert(_pAiNodeAnim->mNumPositionKeys > 0 && "���W�L�[����܂���");

	// ���Ԃ���ȑO�̃L�[���擾
	for (u_int k_i = 0; k_i < _pAiNodeAnim->mNumPositionKeys - 1; k_i++)
	{
		if (_playingTime < _pAiNodeAnim->mPositionKeys[k_i + 1].mTime)
			return k_i;
	}

	assert(!"���W�L�[������Ɏ擾�ł��܂���ł���");
	return 0;
}

DirectX::SimpleMath::Vector3 AnimationData::GetScaleByKey(u_int _nodeId, u_int _key) const
{
	aiNodeAnim* aiNodeAnim = pAnimationData->mChannels[_nodeId];

	// �L�[����葽��������������
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

	// �L�[����葽��������������
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

	// �L�[����葽��������������
	assert(aiNodeAnim->mNumPositionKeys > _key);

	aiVector3D aiPos = aiNodeAnim->mPositionKeys[_key].mValue;

	Vector3 pos;
	pos.x = aiPos.x;
	pos.y = aiPos.y;
	pos.z = aiPos.z;

	return pos;
}