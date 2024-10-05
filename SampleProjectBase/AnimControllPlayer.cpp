#include "pch.h"
#include "AnimControllPlayer.h"
#include "AnimationController.h"
#include "AnimSingleNodePlayer.h"
#include "BlendNodePlayer.h"

AnimControllPlayer::AnimControllPlayer(const AnimationController& _animController, BoneList& _boneList, Transform& _transform)
	: pAnimController(&_animController), pNodeInfo(nullptr), pBoneList(&_boneList), pObjectTransform(&_transform), playSpeed(1.0f), playState(PlayState::PlayNode)
{
	PlayInit();
}

void AnimControllPlayer::Update()
{
	switch (playState)
	{
	case AnimControllPlayer::PlayState::PlayNode:
		NodePlay();
		break;

	case AnimControllPlayer::PlayState::Transition:

		break;
	default:
		break;
	}
	
}

void AnimControllPlayer::ChangeNodeInfo(const AnimationController::AnimNodeInfo& _changeNodeInfo)
{
	AnimationNode_Base& animNode = *_changeNodeInfo.pAnimNode;

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

AnimationParameters& AnimControllPlayer::GetCopyAnimParameters()
{
	return *pCopyAnimParameters;
}

void AnimControllPlayer::PlayInit()
{
	// �A�j���[�V�����p�����[�^�R�s�[
	const AnimationParameters& originParams = pAnimController->GetAnimationParameters();
	pCopyAnimParameters = std::make_unique<AnimationParameters>(originParams);

	// �f�t�H���g�m�[�h����Đ����n�߂�
	ChangeNodeInfo(*pAnimController->GetDefaultNode());

	// �m�[�h�Đ�����n�߂�
	playState = PlayState::PlayNode;
}

void AnimControllPlayer::NodePlay()
{
	pCurNodePlayer->UpdateCall(playSpeed);
}

void AnimControllPlayer::ImGuiSetting()
{
	pCopyAnimParameters->ImGuiCall();
}

