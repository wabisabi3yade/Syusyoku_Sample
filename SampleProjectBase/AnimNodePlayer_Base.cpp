#include "pch.h"
#include "AnimNodePlayer_Base.h"
#include "SkeletalMesh.h"
#include "AnimationNode_Base.h"

AnimNodePlayer_Base::AnimNodePlayer_Base(const AnimationNode_Base& _playNode, BoneList& _boneList, Transform& _transform)
	: pPlayAnimNode(&_playNode), pBoneList(&_boneList), pObjectTransform(&_transform), curPlayRatio(0.0f), lastPlayRatio(-Mathf::smallValue), isJustLoop(false)
{
	// �m�[�h�݂̂̍Đ����x�����߂�
	playNodeSpeedTimes = 1.0f / pPlayAnimNode->GetAnimationTime();
}

void AnimNodePlayer_Base::UpdateCall(std::vector<BoneTransform>& _outTransforms, float _controllerPlaySpeed)
{
	// �Đ�������i�߂�
	ProgressPlayRatio(_controllerPlaySpeed);

	// �A�j���[�V�����̍X�V����
	Update(_outTransforms);

	// ���[�g���[�V�����̍��W�ړ����x���v�Z
	CalcRootMotionPosSpeed(_controllerPlaySpeed);

	// �A�j���[�V�����̃��[�g���[�V������K�p����
	ApplyRootMotionToTransform();
}

void AnimNodePlayer_Base::SetCurPlayRatio(float _playRatio)
{
	curPlayRatio = _playRatio;

	// 1�t���[���O�̍Đ����������݂̊������O�ɒu��
	lastPlayRatio = curPlayRatio - Mathf::smallValue;
}
void AnimNodePlayer_Base::SetPlaySpeedTimes(float _playSpeed)
{
	playNodeSpeedTimes = _playSpeed;
}

float AnimNodePlayer_Base::CalcPlaySpeed(float _controllerSpeed) const
{
	return _controllerSpeed * playNodeSpeedTimes * pPlayAnimNode->GetPlaySpeedTimes();
}

float AnimNodePlayer_Base::GetCurPlayRatio() const
{
	return curPlayRatio;
}

float AnimNodePlayer_Base::GetLastPlayRatio() const
{
	return lastPlayRatio;
}

float AnimNodePlayer_Base::GetNodePlaySpeed() const
{
	return playNodeSpeedTimes;
}

void AnimNodePlayer_Base::GetDeltaRootPos(DirectX::SimpleMath::Vector3& _outPos) const
{
	_outPos = GetRootMotionPos(curPlayRatio) - p_RootMotionPos;
}

void AnimNodePlayer_Base::GetCurrentRootPos(DirectX::SimpleMath::Vector3& _outPos, bool _isLoadScaling) const
{
	_outPos = GetRootMotionPos(GetCurPlayRatio(), _isLoadScaling);
}

const std::string& AnimNodePlayer_Base::GetNodeName() const
{
	return pPlayAnimNode->GetNodeName();
}

const DirectX::SimpleMath::Vector3& AnimNodePlayer_Base::GetRootMotionSpeed() const
{
	return rootMotionPosSpeedPerSec;
}

void AnimNodePlayer_Base::ProgressPlayRatio(float _controllerPlaySpeed)
{
	isJustLoop = false;

	// 1�t���[���O�̍Đ��������X�V
	lastPlayRatio = curPlayRatio;

	/*
	�R���g���[���S�̂̍Đ����x �~
	�m�[�h�̍Đ����x�@�~
	�m�[�h�̑��x�{���@�~
	DeletTime
	*/
	curPlayRatio += CalcPlaySpeed(_controllerPlaySpeed) * MainApplication::DeltaTime();

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

	isJustLoop = true;
	curPlayRatio = Mathf::Repeat(curPlayRatio, 1.0f);	// �Đ�������߂�
}

void AnimNodePlayer_Base::ApplyRootMotionToTransform()
{
	using namespace DirectX::SimpleMath;

	if (isJustLoop)	// ���[�v�����Ȃ�
	{
		// �O��̃��[�g���[�V����������������
		p_RootMotionPos = GetRootMotionPos(0.0f);
		p_RootMotionRot = GetRootMotionRot(0.0f);
	}

	float curPlayRatio = GetCurPlayRatio();

	// �ړ����W
	Vector3 curPos = GetRootMotionPos(curPlayRatio);
	Vector3 posRootMovemrnt = curPos - p_RootMotionPos;

	// ���[�v���ɑO��̍Đ���������A�j���[�V�����Ō�܂ł̃��[�g���[�V�����̍��W�ړ�
	Vector3 loopDeadRMDistabce;
	if (curPlayRatio < lastPlayRatio)
	{
		Vector3 endRootMotionPos = GetRootMotionPos(1.0f);
		loopDeadRMDistabce = endRootMotionPos - GetRootMotionPos(lastPlayRatio);
	}

	// �I�u�W�F�N�g�̌����ɔ��f����
	Vector3 worldMovement;
	if (pPlayAnimNode->GetIsRootMotionXZ())
	{
		worldMovement += pObjectTransform->Right() * (posRootMovemrnt.x + loopDeadRMDistabce.x);
		worldMovement += pObjectTransform->Forward() * (posRootMovemrnt.z + loopDeadRMDistabce.z);
	}
	if (pPlayAnimNode->GetIsRootMotionY())
		worldMovement += pObjectTransform->Up() * (posRootMovemrnt.y + loopDeadRMDistabce.y);

	// �I�u�W�F�N�g�̍��W�X�V
	pObjectTransform->SetPosition(pObjectTransform->GetPosition() + worldMovement);
	p_RootMotionPos = curPos;
}

void AnimNodePlayer_Base::ApplyLoadTransform(DirectX::SimpleMath::Vector3& _rootMotionPos) const
{
	using namespace DirectX::SimpleMath;

	_rootMotionPos *= pBoneList->GetLoadScale();
	_rootMotionPos = Vector3::Transform(_rootMotionPos, Matrix::CreateFromQuaternion(pBoneList->GetLoadRotation()));
}

void AnimNodePlayer_Base::ImGuiSetting()
{
	ImGui::SliderFloat("play", &curPlayRatio, 0.0f, 1.0f);
	ImGui::DragFloat("speed", &playNodeSpeedTimes, 0.01f, 0.0f, 50.0f);
}
