#include "pch.h"
#include "AnimationController.h"
// �m�[�h���
#include "SingleAnimationNode.h"
#include "BlendAnimationNode.h"
// �A�Z�b�g
#include "AssetGetter.h"
// �{�[�����X�g�擾���邽��
#include "SkeletalMesh.h"
// �������
#include "InertInterpAnimation.h"

using namespace DirectX::SimpleMath;

AnimationController::AnimationController() : pCurrentAnimNode(nullptr), pNextAnimNode(nullptr), isTransitioning(false)
{
	inertInterp = std::make_unique<InertInterpAnimation>();
}

void AnimationController::Update(BoneList& _boneList, float _playingRatio)
{
	if (isTransitioning)	// �A�j���[�V�����J�ڒ��Ȃ�
		TransitionUpdate(_boneList, _playingRatio);
	else
		NormalUpdate(_boneList, _playingRatio);

	// �{�[���̃L���b�V���擾�̂��߃p�����[�^���擾����
	std::vector<BoneTransform> cacheTransforms;
	for (u_int b_i = 0; b_i < _boneList.GetBoneCnt(); b_i++)
	{
		cacheTransforms.push_back(_boneList.GetBone(b_i).GetAnimationTransform());
	}
	// ������ԗp�̃L���b�V�����X�V����
	inertInterp->UpdateBoneCache(cacheTransforms);
}

void AnimationController::NormalUpdate(BoneList& _boneList, float& _playingRatio)
{
	pCurrentAnimNode->Update(_playingRatio, _boneList);
}

void AnimationController::TransitionUpdate(BoneList& _boneList, float& _playingTime)
{
	blendElapsedTime += MainApplication::DeltaTime();

	if (blendElapsedTime > blendTime)	// �u�����h���ԉ߂�����
	{
		// �J�ڏI���̏���
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

	for (auto& a : pAnimationNodes)	// �{�^���ŃA�j���[�V�����ς���
	{
		std::string animName = a.second->GetNodeName();
		if(ImGui::Button(animName.c_str()))
		{
			ChangeAnimation(animName);	// �A�j���[�V�����ύX
		}

		a.second->ImGuiSetting();
	}

	ImGui::Text(TO_UTF8("�J�ڏ��"));

	if (isTransitioning)
		ImGuiTransition();

}

void AnimationController::ChangeAnimation(const std::string& _animName)
{
	if (!IsHaveAnim(_animName))
	{
		HASHI_DEBUG_LOG(_animName + "�͎擾�ł��܂���ł���");
		return;
	}

	pNextAnimNode = pAnimationNodes[_animName].get();

	//// ���̃A�j���[�V�����̎p�����擾����
	//std::vector<BoneTransform> requestPose;

	//// ������Ԃ̏�������
	//bool isSuccess = inertInterp->Calculate(*pNextAnimNode, blendTime);

	//if (isSuccess)	// �������Ă�����
	//{
		isTransitioning = true;
		blendElapsedTime = 0.0f;
	//}
}

void AnimationController::AddAnimation(const std::string& _animName)
{
	// �����A�j���[�V��������������
	if (IsHaveAnim(_animName))	return;

	AnimationData* pGetAnim = AssetGetter::GetAsset<AnimationData>(_animName);

	// �A�j���[�V�������Ȃ�������
	if (pGetAnim == nullptr) return;

	// �m�[�h���쐬����
	std::unique_ptr<SingleAnimationNode> pAnimNode = std::make_unique<SingleAnimationNode>(_animName);
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

	text = "�o�߁F" + std::to_string(blendElapsedTime) + "(s)";
	ImGui::Text(TO_UTF8(text));

	text = "�u�����h����" + std::to_string(blendTime) + "(s)";
	ImGui::Text(TO_UTF8(text));
}

void AnimationController::ImGuiImportAnim()
{
	if(!ImGui::TreeNode("Import")) return;
	
	// �V���O���m�[�h
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

	// �u�����h�m�[�h
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
	// �z����ɓ������O�����邩
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
