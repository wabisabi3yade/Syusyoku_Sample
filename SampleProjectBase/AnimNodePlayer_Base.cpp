#include "pch.h"
#include "AnimNodePlayer_Base.h"
#include "SkeletalMesh.h"
#include "AnimationNode_Base.h"

AnimNodePlayer_Base::AnimNodePlayer_Base(const AnimationNode_Base& _playNode, BoneList& _boneList, Transform& _transform)
	: pPlayAnimNode(&_playNode), pBoneList(&_boneList), pObjectTransform(&_transform), curPlayRatio(0.0f), lastPlayRatio(-Mathf::smallValue)
{
	// �m�[�h�݂̂̍Đ����x�����߂�
	progressNodeSpeed = 1.0f / pPlayAnimNode->GetAnimationTime();
}

void AnimNodePlayer_Base::UpdateCall(float _controllerPlaySpeed)
{
	// �Đ�������i�߂�
	ProgressPlayRatio(_controllerPlaySpeed);

	// �A�j���[�V�����̍X�V����
	Update();

	// �A�j���[�V�����̃��[�g���[�V������K�p����
	ApplyRootMotionToTransform();
}

void AnimNodePlayer_Base::SetCurPlayRatio(float _playRatio)
{
	curPlayRatio = _playRatio;
}
float AnimNodePlayer_Base::GetCurPlayRatio() const
{
	return curPlayRatio;
}

void AnimNodePlayer_Base::ProgressPlayRatio(float _controllerPlaySpeed)
{
	// 1�t���[���O�̍Đ��������X�V
	lastPlayRatio = curPlayRatio;

	// ���Ԃ�i�߂�
	float nodePlayTimeSpeed = pPlayAnimNode->GetPlaySpeedTimes();

	/*
	�R���g���[���S�̂̍Đ����x �~
	�m�[�h�̍Đ����x�@�~
	�m�[�h�̑��x�{���@�~
	DeletTime
	*/
	curPlayRatio += _controllerPlaySpeed * progressNodeSpeed * nodePlayTimeSpeed * MainApplication::DeltaTime();

	if (IsCanLoop())
		OnPlayLoop();
}

bool AnimNodePlayer_Base::IsCanLoop() const
{
	// �A�j���[�V�����̑S�̎��Ԃ𒴂��Ă��Ȃ��Ȃ�
	if (curPlayRatio < 1.0f) return false;
	if (!pPlayAnimNode->GetIsLoop()) return false;

	return true;
}

void AnimNodePlayer_Base::OnPlayLoop()
{
	using namespace DirectX::SimpleMath;

	curPlayRatio -= 1.0f;	// �Đ�������߂�

	// �O��̃��[�g���[�V����������������
	p_RootMotionPos = Vector3::Zero;
	p_RootMotionRot = Quaternion::Identity;
}

void AnimNodePlayer_Base::ApplyRootMotionToTransform()
{
	using namespace DirectX::SimpleMath;

	float curPlayRatio = GetCurPlayRatio();

	// �ړ����W
	Vector3 curPos = GetRootMotionPos(curPlayRatio);
	Vector3 posRootMovemrnt = curPos - p_RootMotionPos;
	if (pPlayAnimNode->GetIsRootMotionXZ())	// XZ�����Ɉړ������Ȃ��Ȃ�
	{
		posRootMovemrnt.x = 0.0f;
		posRootMovemrnt.z = 0.0f;
	}
	if (!pPlayAnimNode->GetIsRootMotionY())	// Y�����Ɉړ������Ȃ��Ȃ�
	{
		posRootMovemrnt.y = 0.0f;
	}

	// �I�u�W�F�N�g�̌����ɔ��f����
	Vector3 worldMovement;
	worldMovement = pObjectTransform->Right() * posRootMovemrnt.x;
	worldMovement += pObjectTransform->Up() * posRootMovemrnt.y;
	worldMovement += pObjectTransform->Forward() * posRootMovemrnt.z;

	// �I�u�W�F�N�g�̍��W�X�V
	pObjectTransform->SetPosition(pObjectTransform->GetPosition() + worldMovement);
	p_RootMotionPos = curPos;
}
