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
	// 各状態更新処理
	StateUpdate();

	// 遷移するか確認
	TransitionCheck();
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

void AnimControllPlayer::PlayInit()
{
	// 慣性補間クラスを作成しておく(キャッシュ保存の為)
	pInertInterp = std::make_unique<InertInterpAnimation>();

	// アニメーションパラメータコピー
	const AnimationParameters& originParams = pAnimController->GetAnimationParameters();
	pCopyAnimParameters = std::make_unique<AnimationParameters>(originParams);

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
	pCurNodePlayer->CopyNotifys(originNotifys, *pCopyAnimParameters);

	// 遷移条件確認クラスを作成
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
	// 現在のノード再生を前のノード再生に移動する
	pPrevNodePlayer = std::move(pCurNodePlayer);

	const AnimationNode_Base& animNode = _changeNode;
	// ノードの種類に対応した再生クラスを作成
	switch (animNode.GetNodeType())
	{
	case AnimationNode_Base::NodeType::Single:	// 単一のアニメーション
		pCurNodePlayer = std::make_unique<AnimSingleNodePlayer>(animNode, *pBoneList, *pObjectTransform);
		break;

	case AnimationNode_Base::NodeType::Blend:	// ブレンドスペース
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

	// ボーンに適用する
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
	// ノード再生クラスを変更
	const AnimationNode_Base& nextAnimation = _changeArrow.GetToNode();
	ChangeNodePlayer(nextAnimation);

	// 通知イベントをコピーする
	const AnimNotifyList& originNotifys = pAnimController->GetNodeInfo(nextAnimation)->notifyList;
	pCurNodePlayer->CopyNotifys(originNotifys, *pCopyAnimParameters);

	HashiTaku::AnimInterpolateKind interpolate = _changeArrow.GetInterpolateKind();

	// 新しく遷移チェッカーを作成する
	const AnimNodeInfo& nodeInfo = *pAnimController->GetNodeInfo(nextAnimation);
	const auto& transArrowList = nodeInfo.transitionArrows;
	pTransChecker = std::make_unique<AnimTransitionChecker>(*pCopyAnimParameters, transArrowList);

	// 遷移ステートに変更
	updateState = UpdateState::Transition;

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
	// 遷移先のアニメーションの開始割合をセット
	float targetRatio = _changeArrow.GetTargetRatio();
	pCurNodePlayer->SetCurPlayRatio(targetRatio);

	float transTime = _changeArrow.GetTransitionTime();
	HashiTaku::EaseKind easeKind = _changeArrow.GetEaseKind();


	pCrossFadeInterp = std::make_unique<CrossFadeAnimation>();
	pCrossFadeInterp->Begin(*pPrevNodePlayer, *pCurNodePlayer, *pBoneList, transTime, easeKind);
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

