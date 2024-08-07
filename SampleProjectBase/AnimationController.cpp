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

AnimationController::AnimationController() : pCurrentAnimNode(nullptr), pNextAnimNode(nullptr), isTransitioning(false)
{
	inertInterp = std::make_unique<InertInterpAnimation>();
}

void AnimationController::Update(BoneList& _boneList, float _playingRatio)
{
	if (isTransitioning)	// アニメーション遷移中なら
		TransitionUpdate(_boneList, _playingRatio);
	else
		NormalUpdate(_boneList, _playingRatio);

	// ボーンのキャッシュ取得のためパラメータを取得する
	std::vector<BoneTransform> cacheTransforms;
	for (u_int b_i = 0; b_i < _boneList.GetBoneCnt(); b_i++)
	{
		cacheTransforms.push_back(_boneList.GetBone(b_i).GetAnimationTransform());
	}
	// 慣性補間用のキャッシュを更新する
	inertInterp->UpdateBoneCache(cacheTransforms);
}

void AnimationController::NormalUpdate(BoneList& _boneList, float& _playingRatio)
{
	pCurrentAnimNode->Update(_playingRatio, _boneList);
}

void AnimationController::TransitionUpdate(BoneList& _boneList, float& _playingTime)
{
	blendElapsedTime += MainApplication::DeltaTime();

	if (blendElapsedTime > blendTime)	// ブレンド時間過ぎたら
	{
		// 遷移終了の処理
		TransitionEnd(_playingTime);
	}

	/*for (u_int b_i = 0; b_i < _boneList.GetBoneCnt(); b_i++)
	{
		Bone& bone = _boneList.GetBone(b_i);

		BoneTransform transform;

		transform.position = inertInterp->CalcBlendPos(b_i, blendElapsedTime);
		transform.scale = inertInterp->CalcBlendScale(b_i, blendElapsedTime);
		transform.rotation = inertInterp->CalcBlendRot(b_i, blendElapsedTime);

		bone.SetAnimTransform(transform);
	}*/
}

void AnimationController::ImGuiSetting()
{
	
	ImGuiImportAnim();
	
	if (IsSetAnimation())
	{
		pCurrentAnimNode->ImGuiPlaying();
	}

	for (auto& a : pAnimationNodes)	// ボタンでアニメーション変える
	{
		std::string animName = a.second->GetNodeName();
		if(ImGui::Button(animName.c_str()))
		{
			ChangeAnimation(animName);	// アニメーション変更
		}

		a.second->ImGuiSetting();
	}

	ImGui::Text(TO_UTF8("遷移情報"));

	if (isTransitioning)
		ImGuiTransition();

}

void AnimationController::ChangeAnimation(const std::string& _animName)
{
	if (!IsHaveAnim(_animName))
	{
		HASHI_DEBUG_LOG(_animName + "は取得できませんでした");
		return;
	}

	pNextAnimNode = pAnimationNodes[_animName].get();

	//// 次のアニメーションの姿勢を取得する
	//std::vector<BoneTransform> requestPose;

	//// 慣性補間の初期処理
	//bool isSuccess = inertInterp->Calculate(*pNextAnimNode, blendTime);

	//if (isSuccess)	// 成功していたら
	//{
		isTransitioning = true;
		blendElapsedTime = 0.0f;
	//}
}

void AnimationController::AddAnimation(const std::string& _animName)
{
	// 同じアニメーションがあったら
	if (IsHaveAnim(_animName))	return;

	AnimationData* pGetAnim = AssetGetter::GetAsset<AnimationData>(_animName);

	// アニメーションがなかったら
	if (pGetAnim == nullptr) return;

	// ノードを作成する
	std::unique_ptr<SingleAnimationNode> pAnimNode = std::make_unique<SingleAnimationNode>(_animName);
	pAnimNode->SetAnimationData(*pGetAnim);

	pAnimationNodes[_animName] = std::move(pAnimNode);

	if (!IsSetAnimation())	// 再生中のアニメーションがないならセットする
		pCurrentAnimNode = pAnimationNodes[_animName].get();
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
	if(!ImGui::TreeNode("Import")) return;
	
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
			n->SetAnimationData(*a);
			
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

void AnimationController::TransitionEnd(float& _playTime)
{
	_playTime = 0.0f;
	
	pCurrentAnimNode = pNextAnimNode;
	pNextAnimNode = nullptr;
	isTransitioning = false;
}
