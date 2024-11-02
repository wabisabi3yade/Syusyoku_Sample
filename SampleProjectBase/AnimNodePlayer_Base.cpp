#include "pch.h"
#include "AnimNodePlayer_Base.h"
#include "SkeletalMesh.h"
#include "AnimationNode_Base.h"
#include "IAnimParametersSetter.h"
#include "AnimationNotifyFactory.h"

AnimNodePlayer_Base::AnimNodePlayer_Base(const AnimationNode_Base& _playNode, BoneList& _boneList, Transform& _transform)
	: pPlayAnimNode(&_playNode), pBoneList(&_boneList), pObjectTransform(&_transform),
	curPlayRatio(0.0f), lastPlayRatio(-Mathf::smallValue), animationRatio(0.0f), playerSpeedTimes(1.0f), allPlaySpeed(0.0f), isJustLoop(false), isPlay(true)
{
}

void AnimNodePlayer_Base::CopyNotifys(const std::list<std::unique_ptr<AnimationNotify_Base>>& _notifyList, AnimationParameters& _animationParameters)
{
	// �R�s�[����
	for (auto& origin : _notifyList)
	{
		// �N���[���쐬
		auto pCopyNotify = origin->Clone();

		// �A�j���[�V�����p�����[�^���Z�b�g����Ȃ�
		if (IAnimParametersSetter* pAnimParamSetter = 
			dynamic_cast<IAnimParametersSetter*>(pCopyNotify.get()))
		{
			pAnimParamSetter->SetAnimationParameters(_animationParameters);
		}
	
		// �ǉ�
		copyNotifys.push_back(std::move(pCopyNotify));
	}
}

void AnimNodePlayer_Base::UpdateCall(std::vector<BoneTransform>& _outTransforms, float _controllerPlaySpeed)
{
	if (!isPlay)
	{
		Update(_outTransforms);
		return;
	}

	// �Đ�������i�߂�
	ProgressPlayRatio(_controllerPlaySpeed);

	// �A�j���[�V�����̍X�V����
	Update(_outTransforms);

	// ���[�g���[�V�����̍��W�ړ����x���v�Z
	CalcRootMotionPosSpeed(_controllerPlaySpeed);

	// �A�j���[�V�����̃��[�g���[�V������K�p����
	ApplyRootMotionToTransform();

	// �ʒm�C�x���g���X�V
	NotifyUpdate();
}

void AnimNodePlayer_Base::SetCurPlayRatio(float _playRatio)
{
	curPlayRatio = _playRatio;

	// 1�t���[���O�̍Đ����������݂̊������O�ɒu��
	lastPlayRatio = curPlayRatio - Mathf::smallValue;

	animationRatio = pPlayAnimNode->GetCurveValue(curPlayRatio);
}

void AnimNodePlayer_Base::SetPlaySpeedTimes(float _playSpeed)
{
	playerSpeedTimes = _playSpeed;
}

float AnimNodePlayer_Base::GetCurPlayRatio() const
{
	return curPlayRatio;
}

float AnimNodePlayer_Base::GetLastPlayRatio() const
{
	return lastPlayRatio;
}

float AnimNodePlayer_Base::GetAnimationRatio() const
{
	return animationRatio;
}

float AnimNodePlayer_Base::GetNodePlaySpeed() const
{
	return playerSpeedTimes;
}

void AnimNodePlayer_Base::GetDeltaRootPos(DirectX::SimpleMath::Vector3& _outPos) const
{
	_outPos = GetRootMotionPos(animationRatio) - p_RootMotionPos;
}

void AnimNodePlayer_Base::GetCurrentRootPos(DirectX::SimpleMath::Vector3& _outPos, bool _isLoadScaling) const
{
	_outPos = GetRootMotionPos(GetAnimationRatio(), _isLoadScaling);
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
	�v���C���[�̍Đ����x�@�~
	�m�[�h�̑��x�{���@/
	�Đ����Ԃ��l�������A�j���[�V�������x(�����Ői�߂Ă���̂�)
	*/
	allPlaySpeed = _controllerPlaySpeed *
		playerSpeedTimes *
		pPlayAnimNode->GetPlaySpeedTimes() /
		pPlayAnimNode->GetAnimationTime();


	curPlayRatio += allPlaySpeed * MainApplication::DeltaTime();
	// �A�j���[�V�����������v�Z
	animationRatio = pPlayAnimNode->GetCurveValue(curPlayRatio);

	if (IsCanLoop())
		OnPlayLoop();
}

void AnimNodePlayer_Base::OnTerminal()
{
	for (auto& pNotify : copyNotifys)
	{
		pNotify->OnTerminalCall();
	}

	copyNotifys.clear();
}

bool AnimNodePlayer_Base::IsCanLoop()
{
	// �A�j���[�V�����̑S�̎��Ԃ𒴂��Ă��Ȃ��Ȃ�
	if (curPlayRatio < 1.0f) return false;
	if (!pPlayAnimNode->GetIsLoop())
	{
		isPlay = false;
		return false;
	}

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
		//p_RootMotionRot = GetRootMotionRot(0.0f);
	}

	float curPlayRatio = GetAnimationRatio();

	// �ړ����W
	Vector3 curPos = GetRootMotionPos(curPlayRatio);
	Vector3 posRootMovemrnt = curPos - p_RootMotionPos;

	// ���[�v���ɑO��̍Đ���������A�j���[�V�����Ō�܂ł̃��[�g���[�V�����̍��W�ړ�
	Vector3 loopDeadRMDistabce;
	if (isJustLoop)
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

void AnimNodePlayer_Base::NotifyUpdate()
{
	// �S��
	for (auto& pNotify : copyNotifys)
	{
		pNotify->Update(lastPlayRatio, curPlayRatio, isJustLoop);
	}
}

void AnimNodePlayer_Base::ApplyLoadTransform(DirectX::SimpleMath::Vector3& _rootMotionPos) const
{
	using namespace DirectX::SimpleMath;

	_rootMotionPos *= pBoneList->GetLoadScale();
	_rootMotionPos = Vector3::Transform(_rootMotionPos, Matrix::CreateFromQuaternion(pBoneList->GetLoadRotation()));
}

void AnimNodePlayer_Base::ImGuiSetting()
{
	ImGui::SliderFloat("Play", &curPlayRatio, 0.0f, 1.0f);
	ImGui::Text("AnimRatio:%lf", animationRatio);
	ImGui::DragFloat("Speed", &playerSpeedTimes, 0.01f, 0.0f, 50.0f);
}
