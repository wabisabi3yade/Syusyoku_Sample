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

		// 遷移するか確認
		TransitionCheck();

		// 各状態更新処理
		StateUpdate();

		// トリガー変数をリセット
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
		// 慣性補間クラスを作成しておく(キャッシュ保存の為)
		pInertInterp = std::make_unique<InertInterpAnimation>();

		// アニメーションパラメータコピー
		const AnimationParameters& originParams = pAnimController->GetAnimationParameters();
		pCopyAnimParameters = std::make_unique<AnimationParameters>(originParams);

		pChangeAnimSubject = std::make_unique<ChangeAnimSubject>();

		// デフォルトノードから再生を始める
		SetDefault();

		// ノード再生から始める
		updateState = UpdateState::PlayNode;
	}

	void AnimControllPlayer::SetDefault()
	{
		// デフォルトノードを取得
		const AnimNodeInfo& defaultNodeInfo = *pAnimController->GetDefaultNode();

		// ノード再生を変更
		ChangeNodePlayer(*defaultNodeInfo.pAnimNode);

		// 通知イベントをコピーする
		const AnimNotifyList& originNotifys = defaultNodeInfo.notifyList;
		pCurNodePlayer->CopyNotifys(originNotifys, *pCopyAnimParameters, *pObjectTransform);

		// 遷移条件確認クラスを作成
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
		// 現在のノード再生を前のノード再生に移動する
		pPrevNodePlayer = std::move(pCurNodePlayer);

		const AnimationNode_Base& animNode = _changeNode;
		// ノードの種類に対応した再生クラスを作成
		switch (animNode.GetNodeType())
		{
		case AnimationNode_Base::NodeType::Single:	// 単一のアニメーション
			pCurNodePlayer = std::make_unique<AnimSingleNodePlayer>(animNode, *pAssetBoneList, *pObjectTransform);
			break;

		case AnimationNode_Base::NodeType::Blend:	// ブレンドスペース
			pCurNodePlayer = std::make_unique<AnimBlendNodePlayer>(
				*pCopyAnimParameters,
				animNode,
				*pAssetBoneList,
				*pObjectTransform);
			break;

		case AnimationNode_Base::NodeType::Layerd:	// ブレンドスペース
			pCurNodePlayer = std::make_unique<AnimLayerdNodePlayer>(animNode, *pAssetBoneList, *pObjectTransform);
			break;

		default:
			break;
		}

	}

	void AnimControllPlayer::NodePlayUpdate()
	{
		// アニメーションのトランスフォームを再生機能から取得する
		std::vector<BoneTransform> animationTransforms;

		// コントローラーと再生クラスの速度を掛ける
		float multiPlaySpeed = playSpeed * pAnimController->GetPlaySpeed();

		// ノード再生を更新
		pCurNodePlayer->UpdateCall(animationTransforms, multiPlaySpeed * deltaTime, multiPlaySpeed);

		// 再生終了していて、ボーンを取得できなかったら
		if (static_cast<u_int>(animationTransforms.size() != pAssetBoneList->GetBoneCnt()))
			return;

		// ボーンに適用する
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
		// 現在はクロスフェードしか対応していない
		CrossFadeUpdate();
	}

	void AnimControllPlayer::CrossFadeUpdate()
	{
		using namespace DirectX::SimpleMath;

		// コントローラーと再生クラスの速度を掛ける
		float multiPlaySpeed = playSpeed * pAnimController->GetPlaySpeed();

		// クロスフェードを進める
		pCrossFadeInterp->Update(deltaTime * multiPlaySpeed, multiPlaySpeed);

		// ルートモーションもブレンドする
		Vector3 prevRoot;
		pPrevNodePlayer->GetCurrentRootPos(prevRoot, false);
		Vector3 curRoot;
		pCurNodePlayer->GetCurrentRootPos(curRoot, false);

		rootMotionPos = Vector3::Lerp(prevRoot, curRoot, pCrossFadeInterp->GetTransitionWeight());

		// 遷移終了すると
		if (pCrossFadeInterp->GetIsTransitionEnd())
		{
			// 再生ステートに遷移
			updateState = UpdateState::PlayNode;
		}
	}

	void AnimControllPlayer::InertializationUpdate()
	{

	}

	void AnimControllPlayer::TransitionCheck()
	{
		// 遷移条件を満たした矢印があるか確認
		const AnimTransitionArrow* pTransArrow = pTransChecker->TransitonCheck(
			pCurNodePlayer->GetCurPlayRatio(),
			pCurNodePlayer->GetLastPlayRatio()
		);
		if (!pTransArrow) return;

		// アニメーションを遷移する
		OnChangeAnimation(*pTransArrow);
	}

	void AnimControllPlayer::OnChangeAnimation(const AnimTransitionArrow& _changeArrow)
	{
		// 次のアニメーションノード取得
		const AnimationNode_Base& nextAnimation = _changeArrow.GetToNode();
		AnimInterpolateKind interpolate = _changeArrow.GetInterpolateKind();

		// 新しく遷移チェッカーを作成する
		const AnimNodeInfo& nodeInfo = *pAnimController->GetNodeInfo(nextAnimation);
		const auto& transArrowList = nodeInfo.transitionArrows;
		const auto* groupArrows = pAnimController->GetGroupArrows(nodeInfo.groupArrowsName);
		pTransChecker = std::make_unique<AnimTransitionChecker>(
			*pCopyAnimParameters,
			transArrowList,
			groupArrows);

		// 遷移ステートに変更
		updateState = UpdateState::Transition;

		// 変更前に終了処理を行う
		pCurNodePlayer->OnTerminal();

		// ノード再生クラスを変更
		ChangeNodePlayer(nextAnimation);

		// 通知イベントをコピーする
		const AnimNotifyList& originNotifys = pAnimController->GetNodeInfo(nextAnimation)->notifyList;
		pCurNodePlayer->CopyNotifys(originNotifys, *pCopyAnimParameters, *pObjectTransform);

		// サブジェクト更新
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
		// 遷移先のアニメーションの開始割合をセット
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
		// アニメーション変更をオブザーバーに通知
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