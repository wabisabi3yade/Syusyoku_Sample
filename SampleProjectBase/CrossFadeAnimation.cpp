#include "pch.h"
#include "CrossFadeAnimation.h"

#include "Bone.h"
#include "AnimationNode_Base.h"
#include "SkeletalMesh.h"

using namespace DirectX::SimpleMath;
using namespace HashiTaku;

CrossFadeAnimation::CrossFadeAnimation()
	: pFromNode(nullptr), pToNode(nullptr), transitionWeight(0.0f), transitionTime(0.0f), elapsedTime(0.0f), easeKind(HashiTaku::EaseKind::InOutCubic)
{
}

void CrossFadeAnimation::Begin(AnimationNode_Base& _fromNode, AnimationNode_Base& _toNode, float _transitionTime, HashiTaku::EaseKind _easeKind)
{
	pFromNode = &_fromNode;
	pToNode = &_toNode;

	elapsedTime = 0.0f;
	transitionTime = _transitionTime;
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

void CrossFadeAnimation::ProgressTime(float _deltaTime)
{
	// �J�ڎ��Ԃ𒴂��Ȃ��悤�ɁA���Ԃ�i�߂�
	elapsedTime += _deltaTime;
	elapsedTime = std::min(elapsedTime, transitionTime);

	// �J�ڊ������C�[�W���O���l�������l�ŋ��߂�
	float ratio = elapsedTime / transitionTime;
	transitionWeight = Easing::EaseValue(ratio, easeKind);
}

void CrossFadeAnimation::Update(BoneList& _updateBones, float _playSpeed)
{
	// �A�j���[�V�����Đ��̑��x���l������
	float deltaTime = MainApplication::DeltaTime() * _playSpeed;

	ProgressTime(deltaTime);

	// ������i�߂�
	pFromNode->ProgressPlayRatio(_playSpeed);

	// ������i�߂�
	pToNode->ProgressPlayRatio(_playSpeed);

	// ��Ԃ����g�����X�t�H�[�����{�[���ɓK�p������
	u_int boneCnt = _updateBones.GetBoneCnt();
	for (u_int b_i = 0; b_i < boneCnt; b_i++)
	{
		BoneTransform fromTransform;
	/*	pFromNode->GetCurAnimTransform(fromTransform, b_i);*/

		BoneTransform toTransform;
		/*pToNode->GetCurAnimTransform(toTransform, b_i);*/

		BoneTransform interpTransform;
		Interpolate(fromTransform, toTransform, interpTransform);

		Bone& bone = _updateBones.GetBone(b_i);
		bone.SetAnimTransform(interpTransform);
	}
}
