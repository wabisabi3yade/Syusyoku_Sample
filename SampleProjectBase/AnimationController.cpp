#include "pch.h"
#include "AnimationController.h"

// �A�Z�b�g
#include "AssetGetter.h"

// ImGui
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

void AnimationController::ImGuiSetting()
{
	if (!ImGui::TreeNode(TO_UTF8("�A�j���[�V�����R���g���[���["))) return;

	std::string text = "���O�F";
	if (IsSetAnimation())
	{
		const AnimationData& animData = pCurrentAnimNode->GetAnimationData();
		text += animData.GetAssetName();
		ImGui::Text(TO_UTF8(text));

		text = "���ԁF" + std::to_string(animData.GetAnimationTime());
		ImGui::Text(TO_UTF8(text));
	}

	ImGui::Text(TO_UTF8("�A�j���[�V�����J��"));
	for (auto& a : pAnimationNodes)	// �{�^���ŃA�j���[�V�����ς���
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
		HASHI_DEBUG_LOG(_animName + "�͎擾�ł��܂���ł���");
		return;
	}
}

void AnimationController::AddAnimation(const std::string& _animName)
{
	// �����A�j���[�V��������������
	if (IsHaveAnim(_animName))	return;

	AnimationData* pGetAnim = AssetGetter::GetAsset<AnimationData>(_animName);

	// �A�j���[�V�������Ȃ�������
	if (pGetAnim == nullptr) return;

	// �m�[�h���쐬����
	std::unique_ptr<AnimStateNode> pAnimNode = std::make_unique<AnimStateNode>();
	pAnimNode->SetAnimationData(*pGetAnim);

	pAnimationNodes[_animName] = std::move(pAnimNode);

	if (!IsSetAnimation())	// �Đ����̃A�j���[�V�������Ȃ��Ȃ�Z�b�g����
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
	// �z����ɓ������O�����邩
	for (auto& a : pAnimationNodes)
	{
		if (_animName == a.first)
			return true;
	}

	return false;
}
