#include "pch.h"
#include "AnimControllPlayer.h"
#include "AnimationController.h"
#include "AnimSingleNodePlayer.h"
#include "BlendNodePlayer.h"

AnimControllPlayer::AnimControllPlayer(const AnimationController& _animController, BoneList& _boneList, Transform& _transform)
	: pAnimController(&_animController), pBoneList(&_boneList), pObjectTransform(&_transform), playSpeed(1.0f), updateState(UpdateState::PlayNode), curTransitonKind(HashiTaku::AnimInterpolateKind::CrossFade)
{
	PlayInit();
}

void AnimControllPlayer::Update()
{
	// �J�ڂ��邩�m�F
	TransitionCheck();

	// �e��ԍX�V����
	StateUpdate();

	// �g���K�[�ϐ������Z�b�g
	pCopyAnimParameters->ResetTrigger();
}

void AnimControllPlayer::AddChangeAnimObserver(HashiTaku::ChangeAnimObserver& _observer)
{
	pChangeAnimSubject->AddObserver(_observer);
}

void AnimControllPlayer::RemoveChangeAnimObserver(HashiTaku::ChangeAnimObserver& _observer)
{
	pChangeAnimSubject->RemoveObserver(_observer);
}

void AnimControllPlayer::SetCurrentPlaySpeed(float _speed)
{
	playSpeed = _speed;
}

AnimationParameters& AnimControllPlayer::GetCopyAnimParameters()
{
	return *pCopyAnimParameters;
}

AnimNodePlayer_Base& AnimControllPlayer::GetCurNodePlayer() const
{
	return *pCurNodePlayer;
}

void AnimControllPlayer::GetCurrentRootPos(DirectX::SimpleMath::Vector3& _outPos) const
{
	_outPos = rootMotionPos;
}

float AnimControllPlayer::GetCurrentPlaySpeed() const
{
	return playSpeed;
}

void AnimControllPlayer::PlayInit()
{
	// ������ԃN���X���쐬���Ă���(�L���b�V���ۑ��̈�)
	pInertInterp = std::make_unique<InertInterpAnimation>();

	// �A�j���[�V�����p�����[�^�R�s�[
	const AnimationParameters& originParams = pAnimController->GetAnimationParameters();
	pCopyAnimParameters = std::make_unique<AnimationParameters>(originParams);

	pChangeAnimSubject = std::make_unique<HashiTaku::ChangeAnimSubject>();

	// �f�t�H���g�m�[�h����Đ����n�߂�
	SetDefault();

	// �m�[�h�Đ�����n�߂�
	updateState = UpdateState::PlayNode;
}

void AnimControllPlayer::SetDefault()
{
	// �f�t�H���g�m�[�h���擾
	const AnimNodeInfo& defaultNodeInfo = *pAnimController->GetDefaultNode();

	// �m�[�h�Đ���ύX
	ChangeNodePlayer(*defaultNodeInfo.pAnimNode);
	// �ʒm�C�x���g���R�s�[����
	const AnimNotifyList& originNotifys = defaultNodeInfo.notifyList;
	pCurNodePlayer->CopyNotifys(originNotifys, *pCopyAnimParameters);

	// �J�ڏ����m�F�N���X���쐬
	const auto& transArrows = defaultNodeInfo.transitionArrows;
	pTransChecker = std::make_unique<AnimTransitionChecker>(*pCopyAnimParameters, transArrows);
}

void AnimControllPlayer::StateUpdate()
{
	switch (updateState)
	{
	case AnimControllPlayer::UpdateState::PlayNode:
		NodePlayUpdate();
		break;

	case AnimControllPlayer::UpdateState::Transition:
		TransitionUpdate();
		break;

	default:
		break;
	}
}

void AnimControllPlayer::ChangeNodePlayer(const AnimationNode_Base& _changeNode)
{
	// ���݂̃m�[�h�Đ���O�̃m�[�h�Đ��Ɉړ�����
	pPrevNodePlayer = std::move(pCurNodePlayer);

	const AnimationNode_Base& animNode = _changeNode;
	// �m�[�h�̎�ނɑΉ������Đ��N���X���쐬
	switch (animNode.GetNodeType())
	{
	case AnimationNode_Base::NodeType::Single:	// �P��̃A�j���[�V����
		pCurNodePlayer = std::make_unique<AnimSingleNodePlayer>(animNode, *pBoneList, *pObjectTransform);
		break;

	case AnimationNode_Base::NodeType::Blend:	// �u�����h�X�y�[�X
		pCurNodePlayer = std::make_unique<AnimBlendNodePlayer>(*pCopyAnimParameters, animNode, *pBoneList, *pObjectTransform);
		break;

	default:
		break;
	}

}

void AnimControllPlayer::NodePlayUpdate()
{
	std::vector<BoneTransform> animationTransforms;
	pCurNodePlayer->UpdateCall(animationTransforms, playSpeed);

	// �Đ��I�����Ă��āA�{�[�����擾�ł��Ȃ�������
	if(static_cast<u_int>(animationTransforms.size() != pBoneList->GetBoneCnt()))
		return;

	// �{�[���ɓK�p����
	for (u_int b_i = 0; b_i < pBoneList->GetBoneCnt(); b_i++)
	{
		Bone& bone = pBoneList->GetBone(b_i);
		bone.GetRefelenceAnimTransform() = animationTransforms[b_i];
	}
	
	pCurNodePlayer->GetCurrentRootPos(rootMotionPos, false);
}

void AnimControllPlayer::TransitionUpdate()
{
	switch (curTransitonKind)
	{
	case HashiTaku::CrossFade:
		CrossFadeUpdate();
		break;
	case HashiTaku::Inertialization:
		InertializationUpdate();
		break;
	default:
		break;
	}
}

void AnimControllPlayer::CrossFadeUpdate()
{
	using namespace DirectX::SimpleMath;

	pCrossFadeInterp->Update(playSpeed);

	// ���[�g���[�V�������u�����h����
	Vector3 prevRoot;
	pPrevNodePlayer->GetCurrentRootPos(prevRoot, false);
	Vector3 curRoot;
	pCurNodePlayer->GetCurrentRootPos(curRoot, false);

	rootMotionPos = Vector3::Lerp(prevRoot, curRoot, pCrossFadeInterp->GetTransitionWeight());

	// �J�ڏI�������
	if (pCrossFadeInterp->GetIsTransitionEnd())
	{
		// �Đ��X�e�[�g�ɑJ��
		updateState = UpdateState::PlayNode;
	}
}

void AnimControllPlayer::InertializationUpdate()
{

}

void AnimControllPlayer::TransitionCheck()
{
	// �J�ڏ����𖞂�������󂪂��邩�m�F
	const AnimTransitionArrow* pTransArrow = pTransChecker->TransitonCheck(
		pCurNodePlayer->GetAnimationRatio(), 
		pCurNodePlayer->GetLastAnimationRatio()
	);
	if (!pTransArrow) return;

	// �A�j���[�V������J�ڂ���
	OnChangeAnimation(*pTransArrow);
}

void AnimControllPlayer::OnChangeAnimation(const AnimTransitionArrow& _changeArrow)
{
	// ���̃A�j���[�V�����m�[�h�擾
	const AnimationNode_Base& nextAnimation = _changeArrow.GetToNode();
	HashiTaku::AnimInterpolateKind interpolate = _changeArrow.GetInterpolateKind();

	// �V�����J�ڃ`�F�b�J�[���쐬����
	const AnimNodeInfo& nodeInfo = *pAnimController->GetNodeInfo(nextAnimation);
	const auto& transArrowList = nodeInfo.transitionArrows;
	pTransChecker = std::make_unique<AnimTransitionChecker>(*pCopyAnimParameters, transArrowList);

	// �J�ڃX�e�[�g�ɕύX
	updateState = UpdateState::Transition;

	// �ύX�O�ɏI���������s��
	pCurNodePlayer->OnTerminal();

	// �m�[�h�Đ��N���X��ύX
	ChangeNodePlayer(nextAnimation);

	// �ʒm�C�x���g���R�s�[����
	const AnimNotifyList& originNotifys = pAnimController->GetNodeInfo(nextAnimation)->notifyList;
	pCurNodePlayer->CopyNotifys(originNotifys, *pCopyAnimParameters);

	// �T�u�W�F�N�g�X�V
	ChangeAnimSubjectUpdate();

	switch (interpolate)
	{
	case HashiTaku::CrossFade:
		OnCrossFadeBegin(_changeArrow);
		break;

	case HashiTaku::Inertialization:
		break;

	default:
		break;
	}

	
}

void AnimControllPlayer::OnCrossFadeBegin(const AnimTransitionArrow& _changeArrow)
{
	// �J�ڐ�̃A�j���[�V�����̊J�n�������Z�b�g
	float targetRatio = _changeArrow.GetTargetRatio();
	pCurNodePlayer->SetCurPlayRatio(targetRatio);

	float transTime = _changeArrow.GetTransitionTime();
	HashiTaku::EaseKind easeKind = _changeArrow.GetEaseKind();

	pCrossFadeInterp = std::make_unique<CrossFadeAnimation>();
	pCrossFadeInterp->Begin(*pPrevNodePlayer, *pCurNodePlayer, *pBoneList, transTime, easeKind);
}

void AnimControllPlayer::ChangeAnimSubjectUpdate()
{
	// �A�j���[�V�����ύX���I�u�U�[�o�[�ɒʒm
	HashiTaku::ChangeAnimationInfo changeAnimInfo;

	changeAnimInfo.pFromAnimNodeName = &pPrevNodePlayer->GetNodeName();
	changeAnimInfo.pToAnimNodeName = &pCurNodePlayer->GetNodeName();

	pChangeAnimSubject->NotifyAll(changeAnimInfo);
}

void AnimControllPlayer::ImGuiSetting()
{
	ImGui::Text(std::to_string(static_cast<int>(updateState)).c_str());

	if (ImGuiMethod::TreeNode("Current Play"))
	{
		std::string text = "Name:" + pCurNodePlayer->GetNodeName();	ImGui::Text(text.c_str());
		pCurNodePlayer->ImGuiCall();
		ImGui::TreePop();
	}
	

	ImGui::DragFloat("controllPlaySpeed", &playSpeed, 0.01f, 0.0f, 100.0f);
	pCopyAnimParameters->ImGuiCall();
}

