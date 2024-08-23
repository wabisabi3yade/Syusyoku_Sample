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
	: pCurrentAnimNode(nullptr), pNextAnimNode(nullptr), playingRatio(0.0f), playSpeed(1.0f), blendElapsedTime(0.0f), pBoneList(nullptr), isPlay(true), isTransitioning(false)
{
	inertInterp = std::make_unique<InertInterpAnimation>();
}

void AnimationController::Update(BoneList& _boneList)
{
	if (!IsCanPlay()) return;

	// 再生時間を進める
	ProgressPlayTime();

	// アニメーション更新
	AnimatioUpdate(_boneList);
}

void AnimationController::NormalUpdate(BoneList& _boneList)
{
	pCurrentAnimNode->Update(playingRatio, _boneList);
}

void AnimationController::TransitionUpdate(BoneList& _boneList)
{
	blendElapsedTime += MainApplication::DeltaTime();

	if (blendElapsedTime > blendTime)	// ブレンド時間過ぎたら
	{
		// 遷移終了の処理
		InterpTransitionEnd();
	}

	for (u_int b_i = 0; b_i < _boneList.GetBoneCnt(); b_i++)
	{
		Bone& bone = _boneList.GetBone(b_i);

		BoneTransform transform;

		transform.position = inertInterp->CalcBlendPos(b_i, blendElapsedTime);
		transform.scale = inertInterp->CalcBlendScale(b_i, blendElapsedTime);
		transform.rotation = inertInterp->CalcBlendRot(b_i, blendElapsedTime);

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
			ChangeAnimation(animName);	// アニメーション変更
		}
	}

	ImGui::Text(TO_UTF8("遷移情報"));

	if (isTransitioning)
		ImGuiTransition();

}

void AnimationController::ChangeAnimation(const std::string& _animName, bool _isInterp)
{
	if (!IsHaveAnim(_animName))	// アニメーションがなかったら
	{
		HASHI_DEBUG_LOG(_animName + "は取得できませんでした");
		return;
	}

	// 再生開始する
	isPlay = true;

	if (!_isInterp)	// 補間しないなら
	{
		pCurrentAnimNode = pAnimationNodes[_animName].get();
		return;
	}

	// 補間開始する
	InterpTransitionStart(_animName);
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
	std::unique_ptr<SingleAnimationNode> pAnimNode = std::make_unique<SingleAnimationNode>(_nodeName);
	pAnimNode->SetAnimationData(_animName);
	pAnimationNodes[_nodeName] = std::move(pAnimNode);

	if (!IsSetAnimation())	// 再生中のアニメーションがないならセットする
		pCurrentAnimNode = pAnimationNodes[_nodeName].get();
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

void AnimationController::ProgressPlayTime()
{
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

void AnimationController::AnimatioUpdate(BoneList& _boneList)
{
	if (isTransitioning)	// アニメーション遷移中なら
		TransitionUpdate(_boneList);
	else
		NormalUpdate(_boneList);

	// ボーンのキャッシュ取得のためパラメータを取得する
	std::vector<BoneTransform> cacheTransforms;
	for (u_int b_i = 0; b_i < _boneList.GetBoneCnt(); b_i++)
	{
		cacheTransforms.push_back(_boneList.GetBone(b_i).GetAnimationTransform());
	}
	// 慣性補間用のキャッシュを更新する
	inertInterp->UpdateBoneCache(cacheTransforms);
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

void AnimationController::ImGuiTransition()
{
	std::string text = pCurrentAnimNode->GetNodeName();
	text += " ---> ";
	text += pNextAnimNode->GetNodeName();
	ImGui::Text(TO_UTF8(text));

	text = "経過：" + std::to_string(blendElapsedTime) + "(s)";
	ImGui::Text(TO_UTF8(text));

	text = "ブレンド時間" + std::to_string(blendTime) + "(s)";
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

void AnimationController::InterpTransitionStart(const std::string& _animName)
{
	pNextAnimNode = pAnimationNodes[_animName].get();

	// 次のアニメーションの姿勢を取得する
	std::vector<BoneTransform> requestPose;

	pNextAnimNode->GetAnimTransform(requestPose, pBoneList->GetBoneCnt(), 0);

	// 慣性補間の初期処理
	bool isSuccess = inertInterp->Calculate(requestPose, blendTime);

	if (isSuccess)	// 成功していたら
	{
		isTransitioning = true;
		blendElapsedTime = 0.0f;
	}
}

void AnimationController::InterpTransitionEnd()
{
	playingRatio = 0.0f;

	pCurrentAnimNode = pNextAnimNode;
	pNextAnimNode = nullptr;
	isTransitioning = false;
}

void AnimationController::SetBoneList(BoneList& _boneList)
{
	pBoneList = &_boneList;
}

void AnimationController::OnAnimationFinish()
{
	isPlay = false;
	playingRatio = 0.0f;
}

