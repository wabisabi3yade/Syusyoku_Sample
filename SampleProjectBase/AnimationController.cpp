#include "pch.h"
#include "AnimationController.h"

// アセット
#include "AssetGetter.h"

// ImGui
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

void AnimationController::ImGuiSetting()
{
	if (!ImGui::TreeNode(TO_UTF8("アニメーションコントローラー"))) return;

	std::string text = "名前：";
	if (IsSetAnimation())
	{
		const AnimationData& animData = pCurrentAnimNode->GetAnimationData();
		text += animData.GetAssetName();
		ImGui::Text(TO_UTF8(text));

		text = "時間：" + std::to_string(animData.GetAnimationTime());
		ImGui::Text(TO_UTF8(text));
	}

	ImGui::Text(TO_UTF8("アニメーション遷移"));
	for (auto& a : pAnimationNodes)	// ボタンでアニメーション変える
	{
		std::string animName = a.second->GetAnimationData().GetAssetName();
		if(ImGui::Button(animName.c_str()))
		{
			ChangeAnimation(animName);
		}
	}

	ImGui::TreePop();
}

void AnimationController::ChangeAnimation(const std::string& _animName)
{
	if (!IsHaveAnim(_animName))
	{
		HASHI_DEBUG_LOG(_animName + "は取得できませんでした");
		return;
	}
}

void AnimationController::AddAnimation(const std::string& _animName)
{
	// 同じアニメーションがあったら
	if (IsHaveAnim(_animName))	return;

	AnimationData* pGetAnim = AssetGetter::GetAsset<AnimationData>(_animName);

	// アニメーションがなかったら
	if (pGetAnim == nullptr) return;

	// ノードを作成する
	std::unique_ptr<AnimStateNode> pAnimNode = std::make_unique<AnimStateNode>();
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

AnimStateNode& AnimationController::GetCurrentNode()
{
	return *pCurrentAnimNode;
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
