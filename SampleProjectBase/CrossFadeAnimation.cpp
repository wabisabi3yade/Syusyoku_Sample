#include "pch.h"
#include "CrossFadeAnimation.h"

#include "Bone.h"
#include "AnimNodePlayer_Base.h"
#include "SkeletalMesh.h"

using namespace DirectX::SimpleMath;
using namespace HashiTaku;

CrossFadeAnimation::CrossFadeAnimation()
	: pFromNodePlayer(nullptr), pToNodePlayer(nullptr), pAssetBoneList(nullptr), transitionWeight(0.0f), transitionTime(0.0f), elapsedTime(0.0f), easeKind(HashiTaku::EaseKind::InOutCubic)
{
}

void CrossFadeAnimation::Begin(AnimNodePlayer_Base& _fromNode, AnimNodePlayer_Base& _toNode, BoneList& _updateBones, float _transitionTime, HashiTaku::EaseKind _easeKind)
{
	pFromNodePlayer = &_fromNode;
	pToNodePlayer = &_toNode;
	pAssetBoneList = &_updateBones;
	elapsedTime = 0.0f;
	transitionTime = std::max(_transitionTime, Mathf::epsilon);
	easeKind = _easeKind;
}

void CrossFadeAnimation::Interpolate(const BoneTransform& _fromTransforms, const BoneTransform& _toTransforms, BoneTransform& _outTransforms)
{
	// ��Ԍv�Z����

	// ���W
	_outTransforms.position = Vector3::Lerp(_fromTransforms.position, _toTransforms.position, transitionWeight);

	// �X�P�[��
	_outTransforms.scale = Vector3::Lerp(_fromTransforms.scale, _toTransforms.scale, transitionWeight);

	// ��]��
	_outTransforms.rotation = Quaternion::Slerp(_fromTransforms.rotation, _toTransforms.rotation, transitionWeight);
}

bool CrossFadeAnimation::GetIsTransitionEnd() const
{
	return transitionWeight >= 1.0f;
}

float CrossFadeAnimation::GetElapsedTime() const
{
	return elapsedTime;
}

float CrossFadeAnimation::GetTransitionTime() const
{
	return transitionTime;
}

float CrossFadeAnimation::GetTransitionWeight() const
{
	return transitionWeight;
}

void CrossFadeAnimation::ProgressTime(float _deltaTime)
{
	// �J�ڎ��Ԃ𒴂��Ȃ��悤�ɁA���Ԃ�i�߂�
	elapsedTime += _deltaTime;
	elapsedTime = std::min(elapsedTime, transitionTime);

	// �J�ڊ������C�[�W���O���l�������l�ŋ��߂�
	float ratio = elapsedTime / transitionTime;
	transitionWeight = Easing::EaseValue(ratio, easeKind);

	// �͈͂ɂ����߂�
	transitionWeight = std::clamp(transitionWeight, 0.0f, 1.0f);
}

void CrossFadeAnimation::Update(float _deltaTime, float _controllerSpeed)
{
	// �J�ڂ̎��Ԃ�i�߁A�A�j���[�V�����̃E�F�C�g��ω�������
	ProgressTime(_deltaTime);

	// �J�ڌ��̃A�j���[�V�����X�V
	std::vector<BoneTransform> fromBoneTransforms;
	pFromNodePlayer->OnInterpolateUpdate(fromBoneTransforms, _deltaTime, _controllerSpeed);

	// �J�ڐ�̃A�j���[�V�����X�V
	std::vector<BoneTransform> toBoneTransforms;
	pToNodePlayer->OnInterpolateUpdate(toBoneTransforms, _deltaTime, _controllerSpeed);
	
	// ���[�g���[�V�����͑J�ڐ�̂��̂��g�p
	Vector3 toMovement = pToNodePlayer->CalcRootMotionToTransform();
	pToNodePlayer->ApplyRootMotion(toMovement);
	
	// ��Ԃ����g�����X�t�H�[�����{�[���ɓK�p������
	u_int boneCnt = pAssetBoneList->GetBoneCnt();
	for (u_int b_i = 0; b_i < boneCnt; b_i++)
	{
		Bone& bone = *pAssetBoneList->GetBone(b_i);

		BoneTransform interpTransform;
		Interpolate(fromBoneTransforms[b_i], toBoneTransforms[b_i], bone.GetRefelenceAnimTransform());
	}
}
