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

AnimationController::AnimationController()
	: pCurrentAnimNode(nullptr), pNextAnimNode(nullptr), playingRatio(0.0f), playSpeed(1.0f), blendElapsedTime(0.0f), pBoneList(nullptr), isPlay(true), isTransitioning(false)
{
	inertInterp = std::make_unique<InertInterpAnimation>();
}

void AnimationController::Update(BoneList& _boneList)
{
	if (!IsCanPlay()) return;

	// �Đ����Ԃ�i�߂�
	ProgressPlayTime();

	// �A�j���[�V�����X�V
	AnimatioUpdate(_boneList);
}

void AnimationController::NormalUpdate(BoneList& _boneList)
{
	pCurrentAnimNode->Update(playingRatio, _boneList);
}

void AnimationController::TransitionUpdate(BoneList& _boneList)
{
	blendElapsedTime += MainApplication::DeltaTime();

	if (blendElapsedTime > blendTime)	// �u�����h���ԉ߂�����
	{
		// �J�ڏI���̏���
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
	ImGui::Text(TO_UTF8(std::string("�Đ����� " + std::to_string(playingRatio))));

	if (IsSetAnimation())
	{
		std::string timeStr = std::to_string(playingRatio * pCurrentAnimNode->GetAnimationTime());
		ImGui::Text(TO_UTF8("�Đ����� " + timeStr));
	}

	ImGui::Checkbox("Play", &isPlay);
	ImGui::DragFloat("PlaySpeed", &playSpeed, 0.1f);

	ImGuiImportAnim();

	if (IsSetAnimation())
	{
		pCurrentAnimNode->ImGuiPlaying();
	}

	for (auto& a : pAnimationNodes)	// �{�^���ŃA�j���[�V�����ς���
	{
		std::string animName = a.second->GetNodeName();
		if (ImGui::Button(animName.c_str()))
		{
			ChangeAnimation(animName);	// �A�j���[�V�����ύX
		}
	}

	ImGui::Text(TO_UTF8("�J�ڏ��"));

	if (isTransitioning)
		ImGuiTransition();

}

void AnimationController::ChangeAnimation(const std::string& _animName, bool _isInterp)
{
	if (!IsHaveAnim(_animName))	// �A�j���[�V�������Ȃ�������
	{
		HASHI_DEBUG_LOG(_animName + "�͎擾�ł��܂���ł���");
		return;
	}

	// �Đ��J�n����
	isPlay = true;

	if (!_isInterp)	// ��Ԃ��Ȃ��Ȃ�
	{
		pCurrentAnimNode = pAnimationNodes[_animName].get();
		return;
	}

	// ��ԊJ�n����
	InterpTransitionStart(_animName);
}

void AnimationController::SetBlendRatio(float _ratio)
{
	using enum AnimationNode_Base::NodeType;

	// ���݂̃m�[�h���u�����h�łȂ��Ȃ�
	if (pCurrentAnimNode->GetNodeType() != Blend) return;

	BlendAnimationNode* pBlend = static_cast<BlendAnimationNode*>(pCurrentAnimNode);

	pBlend->SetTargetBlendRatio(_ratio);
}

void AnimationController::CreateSingleNode(const std::string& _nodeName, const std::string& _animName)
{
	// �����A�j���[�V��������������
	if (IsHaveAnim(_nodeName))	return;

	// �A�j���[�V�������Z�b�g���B�m�[�h��z��ɓ����
	std::unique_ptr<SingleAnimationNode> pAnimNode = std::make_unique<SingleAnimationNode>(_nodeName);
	pAnimNode->SetAnimationData(_animName);
	pAnimationNodes[_nodeName] = std::move(pAnimNode);

	if (!IsSetAnimation())	// �Đ����̃A�j���[�V�������Ȃ��Ȃ�Z�b�g����
		pCurrentAnimNode = pAnimationNodes[_nodeName].get();
}

void AnimationController::CreateBlendNode(const std::vector<std::string>& _animNames,
	const std::vector<float>& _ratios, const std::string& _nodeName)
{
	assert(_animNames.size() == _ratios.size() && "���O�Ɗ����̗v�f�����Ⴂ�܂�");

	std::unique_ptr<BlendAnimationNode> pCreateBlend = std::make_unique<BlendAnimationNode>(_nodeName);

	u_int maxNum = static_cast<u_int>(_animNames.size());

	// �u�����h�A�j���[�V�����ɃA�j���[�V������ǉ�����
	for (u_int n_i = 0; n_i < maxNum; n_i++)
	{
		pCreateBlend->SetAnimationData(_animNames[n_i]);

		// �u�����h�������Z�b�g
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
	// �e�A�j���[�V�����̊�����i�߂鑬�x
	float progressRatioSpeed = 1.0f / pCurrentAnimNode->GetAnimationTime();

	// ���Ԃ�i�߂�
	playingRatio += progressRatioSpeed * playSpeed * MainApplication::DeltaTime();

	if (IsCanLoop())	// ���[�v�ł���Ȃ�
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
	if (isTransitioning)	// �A�j���[�V�����J�ڒ��Ȃ�
		TransitionUpdate(_boneList);
	else
		NormalUpdate(_boneList);

	// �{�[���̃L���b�V���擾�̂��߃p�����[�^���擾����
	std::vector<BoneTransform> cacheTransforms;
	for (u_int b_i = 0; b_i < _boneList.GetBoneCnt(); b_i++)
	{
		cacheTransforms.push_back(_boneList.GetBone(b_i).GetAnimationTransform());
	}
	// ������ԗp�̃L���b�V�����X�V����
	inertInterp->UpdateBoneCache(cacheTransforms);
}

bool AnimationController::IsCanLoop()
{
	// �A�j���[�V�����̑S�̎��Ԃ𒴂��Ă��Ȃ��Ȃ�
	if (playingRatio < 1.0f) return false;

	// ���[�v�Đ����Ȃ��Ȃ�
	if (!pCurrentAnimNode->GetIsLoop())
	{
		// �A�j���[�V�����I������
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

	text = "�o�߁F" + std::to_string(blendElapsedTime) + "(s)";
	ImGui::Text(TO_UTF8(text));

	text = "�u�����h����" + std::to_string(blendTime) + "(s)";
	ImGui::Text(TO_UTF8(text));
}

void AnimationController::ImGuiImportAnim()
{
	if (!ImGui::TreeNode("Import")) return;

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
			n->SetAnimationData(singleStr);

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

void AnimationController::InterpTransitionStart(const std::string& _animName)
{
	pNextAnimNode = pAnimationNodes[_animName].get();

	// ���̃A�j���[�V�����̎p�����擾����
	std::vector<BoneTransform> requestPose;

	pNextAnimNode->GetAnimTransform(requestPose, pBoneList->GetBoneCnt(), 0);

	// ������Ԃ̏�������
	bool isSuccess = inertInterp->Calculate(requestPose, blendTime);

	if (isSuccess)	// �������Ă�����
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

