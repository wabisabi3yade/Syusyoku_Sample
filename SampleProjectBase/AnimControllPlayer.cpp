#include "pch.h"
#include "AnimControllPlayer.h"
#include "AnimationController.h"
#include "AnimSingleNodePlayer.h"
#include "AnimBlendNodePlayer.h"
#include "AnimLayerdNodePlayer.h"

namespace HashiTaku
{
	AnimControllPlayer::AnimControllPlayer(const AnimationController& _animController, BoneList& _boneList, Transform& _transform) :
		pAnimController(&_animController), pAssetBoneList(&_boneList),
		pObjectTransform(&_transform), deltaTime(0.0f), playSpeed(1.0f),
		updateState(UpdateState::PlayNode),
		curTransitonKind(AnimInterpolateKind::CrossFade)
	{
		PlayInit();
	}

	void AnimControllPlayer::Update(float _deltaTime)
	{
		deltaTime = _deltaTime;

		// �J�ڂ��邩�m�F
		TransitionCheck();

		// �e��ԍX�V����
		StateUpdate();

		// �g���K�[�ϐ������Z�b�g
		pCopyAnimParameters->ResetTrigger();
	}

	void AnimControllPlayer::AddChangeAnimObserver(ChangeAnimObserver& _observer)
	{
		pChangeAnimSubject->AddObserver(_observer);
	}

	void AnimControllPlayer::RemoveChangeAnimObserver(ChangeAnimObserver& _observer)
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

		pChangeAnimSubject = std::make_unique<ChangeAnimSubject>();

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
		pCurNodePlayer->CopyNotifys(originNotifys, *pCopyAnimParameters, *pObjectTransform);

		// �J�ڏ����m�F�N���X���쐬
		const auto& transArrows = defaultNodeInfo.transitionArrows;
		const auto* groupArrows = pAnimController->GetGroupArrows(defaultNodeInfo.groupArrowsName);
		pTransChecker = std::make_unique<AnimTransitionChecker>(*pCopyAnimParameters, transArrows,
			groupArrows);
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
			pCurNodePlayer = std::make_unique<AnimSingleNodePlayer>(animNode, *pAssetBoneList, *pObjectTransform);
			break;

		case AnimationNode_Base::NodeType::Blend:	// �u�����h�X�y�[�X
			pCurNodePlayer = std::make_unique<AnimBlendNodePlayer>(
				*pCopyAnimParameters,
				animNode,
				*pAssetBoneList,
				*pObjectTransform);
			break;

		case AnimationNode_Base::NodeType::Layerd:	// �u�����h�X�y�[�X
			pCurNodePlayer = std::make_unique<AnimLayerdNodePlayer>(animNode, *pAssetBoneList, *pObjectTransform);
			break;

		default:
			break;
		}

	}

	void AnimControllPlayer::NodePlayUpdate()
	{
		// �A�j���[�V�����̃g�����X�t�H�[�����Đ��@�\����擾����
		std::vector<BoneTransform> animationTransforms;

		// �R���g���[���[�ƍĐ��N���X�̑��x���|����
		float multiPlaySpeed = playSpeed * pAnimController->GetPlaySpeed();

		// �m�[�h�Đ����X�V
		pCurNodePlayer->UpdateCall(animationTransforms, multiPlaySpeed * deltaTime, multiPlaySpeed);

		// �Đ��I�����Ă��āA�{�[�����擾�ł��Ȃ�������
		if (static_cast<u_int>(animationTransforms.size() != pAssetBoneList->GetBoneCnt()))
			return;

		// �{�[���ɓK�p����
		u_int boneCnt = pAssetBoneList->GetBoneCnt();
		for (u_int b_i = 0; b_i < boneCnt; b_i++)
		{
			Bone& bone = *pAssetBoneList->GetBone(b_i);
			bone.GetRefelenceAnimTransform() = animationTransforms[b_i];
		}

		pCurNodePlayer->GetCurrentRootPos(rootMotionPos, false);
	}

	void AnimControllPlayer::TransitionUpdate()
	{
		// ���݂̓N���X�t�F�[�h�����Ή����Ă��Ȃ�
		CrossFadeUpdate();
	}

	void AnimControllPlayer::CrossFadeUpdate()
	{
		using namespace DirectX::SimpleMath;

		// �R���g���[���[�ƍĐ��N���X�̑��x���|����
		float multiPlaySpeed = playSpeed * pAnimController->GetPlaySpeed();

		// �N���X�t�F�[�h��i�߂�
		pCrossFadeInterp->Update(deltaTime * multiPlaySpeed, multiPlaySpeed);

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
			pCurNodePlayer->GetCurPlayRatio(),
			pCurNodePlayer->GetLastPlayRatio()
		);
		if (!pTransArrow) return;

		// �A�j���[�V������J�ڂ���
		OnChangeAnimation(*pTransArrow);
	}

	void AnimControllPlayer::OnChangeAnimation(const AnimTransitionArrow& _changeArrow)
	{
		// ���̃A�j���[�V�����m�[�h�擾
		const AnimationNode_Base& nextAnimation = _changeArrow.GetToNode();
		AnimInterpolateKind interpolate = _changeArrow.GetInterpolateKind();

		// �V�����J�ڃ`�F�b�J�[���쐬����
		const AnimNodeInfo& nodeInfo = *pAnimController->GetNodeInfo(nextAnimation);
		const auto& transArrowList = nodeInfo.transitionArrows;
		const auto* groupArrows = pAnimController->GetGroupArrows(nodeInfo.groupArrowsName);
		pTransChecker = std::make_unique<AnimTransitionChecker>(
			*pCopyAnimParameters,
			transArrowList,
			groupArrows);

		// �J�ڃX�e�[�g�ɕύX
		updateState = UpdateState::Transition;

		// �ύX�O�ɏI���������s��
		pCurNodePlayer->OnTerminal();

		// �m�[�h�Đ��N���X��ύX
		ChangeNodePlayer(nextAnimation);

		// �ʒm�C�x���g���R�s�[����
		const AnimNotifyList& originNotifys = pAnimController->GetNodeInfo(nextAnimation)->notifyList;
		pCurNodePlayer->CopyNotifys(originNotifys, *pCopyAnimParameters, *pObjectTransform);

		// �T�u�W�F�N�g�X�V
		ChangeAnimSubjectUpdate();

		switch (interpolate)
		{
		case CrossFade:
			OnCrossFadeBegin(_changeArrow);
			break;

		case Inertialization:
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

		if (_changeArrow.GetTransitionTime() == 0.0f)
			int o = 0;

		float transTime = _changeArrow.GetTransitionTime();
		EaseKind easeKind = _changeArrow.GetEaseKind();

		pCrossFadeInterp = std::make_unique<CrossFadeAnimation>();
		pCrossFadeInterp->Begin(*pPrevNodePlayer, *pCurNodePlayer, *pAssetBoneList, transTime, easeKind);
	}

	void AnimControllPlayer::ChangeAnimSubjectUpdate()
	{
		// �A�j���[�V�����ύX���I�u�U�[�o�[�ɒʒm
		ChangeAnimationInfo changeAnimInfo;

		changeAnimInfo.pFromAnimNodeName = &pPrevNodePlayer->GetNodeName();
		changeAnimInfo.pToAnimNodeName = &pCurNodePlayer->GetNodeName();

		pChangeAnimSubject->NotifyAll(changeAnimInfo);
	}

	void AnimControllPlayer::ImGuiDebug()
	{
		ImGui::Text(std::to_string(static_cast<int>(updateState)).c_str());

		if (ImGuiMethod::TreeNode("Node Play"))
		{
			std::string text = "Name:" + pCurNodePlayer->GetNodeName();	ImGui::Text(text.c_str());
			pCurNodePlayer->ImGuiCall();
			ImGui::TreePop();
		}

		ImGui::DragFloat("controllPlaySpeed", &playSpeed, 0.01f, 0.0f, 100.0f);
		pCopyAnimParameters->ImGuiCall();
	}
}