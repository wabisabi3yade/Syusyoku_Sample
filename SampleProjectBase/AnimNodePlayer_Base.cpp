#include "pch.h"
#include "AnimNodePlayer_Base.h"
#include "SkeletalMesh.h"
#include "AnimationNode_Base.h"
#include "IAnimParametersSetter.h"
#include "AnimationNotifyFactory.h"

AnimNodePlayer_Base::AnimNodePlayer_Base(const AnimationNode_Base& _playNode, BoneList& _boneList, Transform& _transform) :
	pPlayAnimNode(&_playNode),
	pAssetBoneList(&_boneList),
	pObjectTransform(&_transform),
	curPlayRatio(0.0f),
	lastPlayRatio(-Mathf::smallValue),
	playerSpeedTimes(1.0f),
	allPlaySpeed(0.0f),
	deltaTime(0.0f),
	isJustLoop(false),
	isPlaying(true)
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

		// ��������
		pCopyNotify->OnInitCall();

		// �ǉ�
		copyNotifys.push_back(std::move(pCopyNotify));
	}
}

void AnimNodePlayer_Base::UpdateCall(std::vector<BoneTransform>& _outTransforms, float _deltaTime, float _controllerSpeed)
{
	deltaTime = _deltaTime;
	lastPlayRatio = curPlayRatio;
	if (!isPlaying)
	{
		Update(_outTransforms);
		return;
	}

	// �Đ�������i�߂�
	ProgressPlayRatio(_controllerSpeed);

	// �A�j���[�V�����̍X�V����
	Update(_outTransforms);

	// ���[�g���[�V�����̍��W�ړ����x���v�Z
	CalcRootMotionPosSpeed();

	// �A�j���[�V�����̃��[�g���[�V������K�p����
	ApplyRootMotion(CalcRootMotionToTransform());

	// �ʒm�C�x���g���X�V
	NotifyUpdate();

	
}

void AnimNodePlayer_Base::ApplyRootMotion(const DirectX::SimpleMath::Vector3& _rootMovement)
{
	pObjectTransform->SetPosition(pObjectTransform->GetPosition() + _rootMovement);
}

void AnimNodePlayer_Base::OnInterpolateUpdate(std::vector<BoneTransform>& _outTransforms, float _deltaTime, float _controllerSpeed)
{
	deltaTime = _deltaTime;
	lastPlayRatio = curPlayRatio;

	if (!isPlaying)
	{
		Update(_outTransforms);
		return;
	}

	// �Đ�������i�߂�
	ProgressPlayRatio(_controllerSpeed);

	// �A�j���[�V�����̍X�V����
	Update(_outTransforms);

	// ���[�g���[�V�����̍��W�ړ����x���v�Z
	CalcRootMotionPosSpeed();

	// �ʒm�C�x���g���X�V
	NotifyUpdate();
}

void AnimNodePlayer_Base::SetCurPlayRatio(float _playRatio)
{
	curPlayRatio = std::clamp(_playRatio, 0.0f, 1.0f);

	// 1�t���[���O�̊��������߂Ă���
	lastPlayRatio = curPlayRatio - Mathf::smallValue;

	// ���[�g���[�V���������߂Ă���
	p_RootMotionPos = GetRootMotionPos(lastPlayRatio);
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

float AnimNodePlayer_Base::GetNodePlaySpeed() const
{
	return playerSpeedTimes;
}

void AnimNodePlayer_Base::GetDeltaRootPos(DirectX::SimpleMath::Vector3& _outPos) const
{
	_outPos = GetRootMotionPos(curPlayRatio) - p_RootMotionPos;
}

void AnimNodePlayer_Base::GetCurrentRootPos(DirectX::SimpleMath::Vector3& _outPos, bool _isLoadScaling) const
{
	_outPos = GetRootMotionPos(curPlayRatio, _isLoadScaling);
}

const std::string& AnimNodePlayer_Base::GetNodeName() const
{
	return pPlayAnimNode->GetNodeName();
}

const DirectX::SimpleMath::Vector3& AnimNodePlayer_Base::GetRootMotionSpeed() const
{
	return rootMotionPosSpeedPerSec;
}

void AnimNodePlayer_Base::ProgressPlayRatio(float _controllerSpeed)
{
	isJustLoop = false;

	// �J�[�u��ł̑��x
	float animCurveSpeed = pPlayAnimNode->GetCurveValue(curPlayRatio);

	/*
	�R���g���[���[���x �~
	�v���C���[�̍Đ����x�@�~
	�A�j���[�V�����J�[�u�ł̑��x�@�~
	�m�[�h�̑��x�{��
	*/
	allPlaySpeed = _controllerSpeed * playerSpeedTimes * 
		pPlayAnimNode->GetPlaySpeedTimes() * animCurveSpeed;

	// 0���Z�h�~
	float divideVal = _controllerSpeed;
	if (divideVal < Mathf::epsilon)
		divideVal = Mathf::epsilon;

	// deltaTime��controllerSpeed���|���Ă���̂�
	float deltaRemoveConSpeed = deltaTime / divideVal;
	curPlayRatio += allPlaySpeed * deltaRemoveConSpeed / pPlayAnimNode->GetAnimationTime();

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
		isPlaying = false;
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

DirectX::SimpleMath::Vector3 AnimNodePlayer_Base::CalcRootMotionToTransform()
{
	using namespace DirectX::SimpleMath;

	if (isJustLoop)	// ���[�v�����Ȃ�
	{
		// �O��̃��[�g���[�V����������������
		p_RootMotionPos = GetRootMotionPos(0.0f);
	}

	// �ړ����W
	Vector3 curPos = GetRootMotionPos(curPlayRatio);
	Vector3 posRootMovemrnt = curPos - p_RootMotionPos;

	// ���[�v���ɑO��̍Đ���������A�j���[�V����1.0�܂ł̃��[�g���[�V�����̍��W�ړ��𑫂�
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

	p_RootMotionPos = curPos;

	return worldMovement;
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

	_rootMotionPos *= pAssetBoneList->GetLoadScale();
	_rootMotionPos = Vector3::Transform(_rootMotionPos, Matrix::CreateFromQuaternion(pAssetBoneList->GetLoadRotation()));
}

void AnimNodePlayer_Base::ImGuiDebug()
{
	ImGui::Checkbox("IsPlay", &isPlaying);
	ImGui::DragFloat("Speed", &playerSpeedTimes, 0.01f, 0.0f, 50.0f);
	float curveSpeed = pPlayAnimNode->GetCurveValue(curPlayRatio);
	ImGui::Text("curveSpeed:%f", curveSpeed);
	ImGui::SliderFloat("Ratio", &curPlayRatio, 0.0f, 1.0f);
	ImGui::Text("LastRatio:%f", lastPlayRatio);
}
