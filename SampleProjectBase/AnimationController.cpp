#include "pch.h"
#include "AnimationController.h"

// ノード種類
#include "SingleAnimationNode.h"
#include "BlendAnimationNode.h"

// アセット
#include "AssetGetter.h"
// ボーンリスト取得するため
#include "SkeletalMesh.h"
// 慣性補間
#include "InertInterpAnimation.h"

using namespace DirectX::SimpleMath;

AnimationController::AnimationController()
	: pCurrentAnimNode(nullptr), pPrevAnimNode(nullptr), playingRatio(0.0f), playSpeed(1.0f), pBoneList(nullptr), isPlay(true), isTransitioning(false)
{
	pInertInterp = std::make_unique<InertInterpAnimation>();
}

void AnimationController::Update(BoneList& _boneList)
{
	if (!IsCanPlay()) return;

	// 再生時間を進める
	ProgressPlayTime();

	// アニメーション更新
	AnimatioUpdate();

	// キャッシュ更新
	CacheUpdate();

	// 遷移の条件を達成しているか確認
	TranstionCheck();
}

void AnimationController::NormalUpdate()
{
	pCurrentAnimNode->UpdateCall(playingRatio, *pBoneList);
}

void AnimationController::TransitionUpdate()
{
	// 遷移の時間を進める
	float transElapsedTime = pInertInterp->ProgressTransitionTime();

	if (pInertInterp->GetIsTransitionEnd())	// ブレンド時間過ぎたら
	{
		// 遷移終了の処理
		InterpTransitionEnd();
		return;
	}

	for (u_int b_i = 0; b_i < pBoneList->GetBoneCnt(); b_i++)
	{
		Bone& bone = pBoneList->GetBone(b_i);

		BoneTransform transform;

		transform.position = pInertInterp->CalcBlendPos(b_i, transElapsedTime);
		transform.scale = pInertInterp->CalcBlendScale(b_i, transElapsedTime);
		transform.rotation = pInertInterp->CalcBlendRot(b_i, transElapsedTime);

		bone.SetAnimTransform(transform);
	}
}

void AnimationController::ImGuiSetting()
{
	ImGui::Text(TO_UTF8(std::string("再生割合 " + std::to_string(playingRatio))));

	if (IsSetAnimation())
	{
		std::string timeStr = std::to_string(playingRatio * pCurrentAnimNode->GetAnimationTime());
		ImGui::Text(TO_UTF8("再生時間 " + timeStr));
	}

	ImGui::Checkbox("Play", &isPlay);
	ImGui::DragFloat("PlaySpeed", &playSpeed, 0.1f);

	ImGuiImportAnim();

	if (IsSetAnimation())
	{
		pCurrentAnimNode->ImGuiPlaying();
	}

	for (auto& a : pAnimationNodes)	// ボタンでアニメーション変える
	{
		std::string animName = a.second->GetNodeName();
		if (ImGui::Button(animName.c_str()))
		{
			ChangeAnimation(animName, 0.0f, 0.2f);	// アニメーション変更
		}
	}

	ImGui::Text(TO_UTF8("遷移情報"));

	if (isTransitioning)
		ImGuiTransition();

}

void AnimationController::ChangeAnimation(const std::string& _animName, float _targetAnimRatio, float _transitionTime)
{
	if (!IsHaveAnim(_animName))	// アニメーションがなかったら
	{
		HASHI_DEBUG_LOG(_animName + "は取得できませんでした");
		return;
	}

	// 補間するか？
	bool isInterp = _transitionTime > Mathf::epsilon;

	// 再生開始する
	isPlay = true;

	// 前回にアニメーションを更新
	pPrevAnimNode = pCurrentAnimNode;

	// 更新する
	pCurrentAnimNode = GetNode(_animName);

	// 開始処理
	pCurrentAnimNode->Begin();

	if (!isInterp)	// 補間しないなら
	{
		OnChangeAnimComplete();
		return;
	}

	// 補間開始する
	InterpTransitionStart(_targetAnimRatio, _transitionTime);
}


void AnimationController::SetBlendRatio(float _ratio)
{
	using enum AnimationNode_Base::NodeType;

	// 現在のノードがブレンドでないなら
	if (pCurrentAnimNode->GetNodeType() != Blend) return;

	BlendAnimationNode* pBlend = static_cast<BlendAnimationNode*>(pCurrentAnimNode);

	pBlend->SetTargetBlendRatio(_ratio);
}

void AnimationController::CreateSingleNode(const std::string& _nodeName, const std::string& _animName)
{
	// 同じアニメーションがあったら
	if (IsHaveAnim(_nodeName))	return;

	// アニメーションをセットし。ノードを配列に入れる
	std::unique_ptr<SingleAnimationNode> pAnimNode 
		= std::make_unique<SingleAnimationNode>(_nodeName);

	pAnimNode->SetAnimationData(_animName);
	pAnimationNodes[_nodeName] = std::move(pAnimNode);
}

void AnimationController::CreateBlendNode(const std::vector<std::string>& _animNames,
	const std::vector<float>& _ratios, const std::string& _nodeName)
{
	assert(_animNames.size() == _ratios.size() && "名前と割合の要素数が違います");

	std::unique_ptr<BlendAnimationNode> pCreateBlend = std::make_unique<BlendAnimationNode>(_nodeName);

	u_int maxNum = static_cast<u_int>(_animNames.size());

	// ブレンドアニメーションにアニメーションを追加する
	for (u_int n_i = 0; n_i < maxNum; n_i++)
	{
		pCreateBlend->SetAnimationData(_animNames[n_i]);

		// ブレンド割合をセット
		pCreateBlend->SetAnimationRatio(_ratios[n_i], _animNames[n_i]);
	}

	pAnimationNodes[_nodeName] = std::move(pCreateBlend);
}

void AnimationController::CreateTransitionArrow(const std::string& _fromNodeName, const std::string& _toNodeName, float _targetAnimRatio, float _transitionTime, std::function<bool()> _condition)
{
	AnimationNode_Base* fromNode = GetNode(_fromNodeName);
	if (!fromNode)
	{
		HASHI_DEBUG_LOG(_fromNodeName + " 遷移元ノードが見つかりませんでした");
		return;
	}

	AnimationNode_Base* toNode = GetNode(_toNodeName);
	if (!toNode)
	{
		HASHI_DEBUG_LOG(_toNodeName + " 遷移先ノードが見つかりませんでした");
		return;
	}

	if (fromNode == toNode) return;	// 自分自身につなげていたら

	// 矢印作成して追加
	std::unique_ptr<AnimTransitionArrow> pTransitionArrow
		= std::make_unique<AnimTransitionArrow>(*fromNode, *toNode, _targetAnimRatio, _transitionTime, _condition);
	fromNode->AddTransitionArrow(std::move(pTransitionArrow));
}

void AnimationController::RemoveAnimation(const std::string& _animName)
{
	if (!IsHaveAnim(_animName)) return;

	pAnimationNodes.erase(_animName);
}

bool AnimationController::IsSetAnimation()
{
	return pCurrentAnimNode != nullptr;
}

AnimationNode_Base* AnimationController::GetCurrentNode()
{
	return pCurrentAnimNode;
}

AnimationNode_Base* AnimationController::GetNode(const std::string& _name)
{
	auto itr = pAnimationNodes.find(_name);

	if (itr == pAnimationNodes.end())
	{
		HASHI_DEBUG_LOG(_name + "が見つかりませんでした");
		return nullptr;
	}
		

	return itr->second.get();
}

void AnimationController::ProgressPlayTime()
{
	if (isTransitioning) return;

	// 各アニメーションの割合を進める速度
	float progressRatioSpeed = 1.0f / pCurrentAnimNode->GetAnimationTime();

	// 時間を進める
	playingRatio += progressRatioSpeed * playSpeed * MainApplication::DeltaTime();

	if (IsCanLoop())	// ループできるなら
		playingRatio = 0.0f;
}

bool AnimationController::IsCanPlay()
{
	if (!isPlay) return false;
	if (pCurrentAnimNode == nullptr) return false;

	return true;
}

void AnimationController::AnimatioUpdate()
{
	if (isTransitioning)	// アニメーション遷移中なら
		TransitionUpdate();
	else
		NormalUpdate();
}

bool AnimationController::IsCanLoop()
{
	// アニメーションの全体時間を超えていないなら
	if (playingRatio < 1.0f) return false;

	// ループ再生しないなら
	if (!pCurrentAnimNode->GetIsLoop())
	{
		// アニメーション終了処理
		OnAnimationFinish();
		return false;
	}

	return true;
}

void AnimationController::CacheUpdate()
{
	// ボーンのキャッシュ取得のためパラメータを取得する
	std::vector<BoneTransform> cacheTransforms;

	for (u_int b_i = 0; b_i < pBoneList->GetBoneCnt(); b_i++)
	{
		cacheTransforms.push_back(pBoneList->GetBone(b_i).GetAnimationTransform());
	}

	// 慣性補間用のキャッシュを更新する
	pInertInterp->UpdateBoneCache(cacheTransforms);
}

void AnimationController::TranstionCheck()
{
	AnimTransitionArrow* pToAnimNode = pCurrentAnimNode->CheckTransition();

	if (!pToAnimNode) return;	// 遷移しないなら終わる

	std::string nextName = pToAnimNode->GetToNode().GetNodeName();	// 遷移先ノード名
	float targetRatio = pToAnimNode->GetTargetRatio();	// 遷移先アニメーション割合
	float transitionTime = pToAnimNode->GetTransitionTime();	// 遷移時間
	ChangeAnimation(nextName, targetRatio, transitionTime);
}

void AnimationController::OnChangeAnimComplete()
{
	HASHI_DEBUG_LOG(pCurrentAnimNode->GetNodeName() + "へ移行");
}

bool AnimationController::IsHaveAnim(const std::string& _animName)
{
	// 配列内に同じ名前があるか
	for (auto& a : pAnimationNodes)
	{
		if (_animName == a.first)
			return true;
	}

	return false;
}

void AnimationController::InterpTransitionStart(float _targetAnimRatio, float _transitionTime)
{
	// 次のアニメーションの姿勢を取得する
	std::vector<BoneTransform> requestPose;
	pCurrentAnimNode->GetAnimTransform(requestPose, pBoneList->GetBoneCnt(), _targetAnimRatio);

	// 慣性補間の初期処理
	bool isSuccess = pInertInterp->Calculate(requestPose, _transitionTime);

	if (isSuccess)	// 成功していたら
	{
		isTransitioning = true;
	}
}

void AnimationController::InterpTransitionEnd()
{
	OnChangeAnimComplete();

	playingRatio = 0.0f;
	isTransitioning = false;	// 遷移終了
}

void AnimationController::SetBoneList(BoneList& _boneList)
{
	pBoneList = &_boneList;
}

void AnimationController::OnAnimationFinish()
{
	isPlay = false;
	playingRatio = 0.0f;
	pCurrentAnimNode->SetFinish();
}

void AnimationController::ImGuiTransition()
{
	std::string text = pPrevAnimNode->GetNodeName();
	text += " ---> ";
	text += pCurrentAnimNode->GetNodeName();
	ImGui::Text(TO_UTF8(text));

	text = "経過：" + std::to_string(pInertInterp->GetTransElapsedTime()) + "(s)";
	ImGui::Text(TO_UTF8(text));

	text = "ブレンド時間" + std::to_string(pInertInterp->GetTransitionTime()) + "(s)";
	ImGui::Text(TO_UTF8(text));
}

void AnimationController::ImGuiImportAnim()
{
	if (!ImGui::TreeNode("Import")) return;

	// シングルノード
	static char singleStr[256] = "";
	ImGui::InputText("animName", singleStr, 256);

	if (ImGui::Button("Import"))
	{
		std::string s = singleStr;
		AnimationData* a = AssetGetter::GetAsset<AnimationData>(s);
		if (a)
		{
			std::string animName = a->GetAssetName();
			std::unique_ptr<SingleAnimationNode> n = std::make_unique<SingleAnimationNode>(animName);
			n->SetAnimationData(singleStr);

			pAnimationNodes[animName] = std::move(n);
		}
	}

	// ブレンドノード
	static char blendNodeName[256] = "";
	ImGui::InputText("nodeName", blendNodeName, 256);

	if (ImGui::Button("MakeBlend"))
	{
		std::unique_ptr<BlendAnimationNode> b = std::make_unique<BlendAnimationNode>(blendNodeName);

		pAnimationNodes[blendNodeName] = std::move(b);
	}

	ImGui::TreePop();
}