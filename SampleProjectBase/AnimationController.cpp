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
using namespace HashiTaku;

AnimationController::AnimationController()
	: pCurrentAnimNode(nullptr), pPrevAnimNode(nullptr), pCurTransArrow(nullptr), playingRatio(0.0f), playSpeed(1.0f), pBoneList(nullptr), isPlay(true), isTransitioning(false)
{
	pCrossFadeInterp = std::make_unique<CrossFadeAnimation>();
	pInertInterp = std::make_unique<InertInterpAnimation>();
}

void AnimationController::Update(BoneList& _boneList)
{
	if (!IsCanPlay()) return;

	// �Đ����Ԃ�i�߂�
	ProgressPlayTime();

	// �A�j���[�V�����X�V
	AnimatioUpdate();

	// �L���b�V���X�V
	CacheUpdate();

	// �J�ڂ̏�����B�����Ă��邩�m�F
	TranstionCheck();
}

void AnimationController::NormalUpdate()
{
	pCurrentAnimNode->UpdateCall(playingRatio, *pBoneList);
}

void AnimationController::CrossFadeUpdate()
{
	pCrossFadeInterp->Update(*pBoneList, playSpeed);

	if (pCrossFadeInterp->GetIsTransitionEnd())
		OnTransitionEnd();
}

void AnimationController::InertInterpUpdate()
{
	// �J�ڂ̎��Ԃ�i�߂�
	float transElapsedTime = pInertInterp->ProgressTransitionTime();

	if (pInertInterp->GetIsTransitionEnd())	// �u�����h���ԉ߂�����
	{
		// �J�ڏI���̏���
		OnTransitionEnd();
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
			ChangeAnimation(animName, nullptr);	// �A�j���[�V�����ύX
		}
	}

	ImGui::Text(TO_UTF8("�J�ڏ��"));

	if (isTransitioning)
		ImGuiTransition();

}

void AnimationController::ChangeAnimation(const std::string& _animName, const AnimTransitionArrow* _transitionArrow)
{
	if (!IsHaveAnim(_animName))	// �A�j���[�V�������Ȃ�������
	{
		HASHI_DEBUG_LOG(_animName + "�͎擾�ł��܂���ł���");
		return;
	}

	// ��Ԃ��邩�H
	bool isInterp = _transitionArrow != nullptr;

	// �ύX���̍Đ�����
	float changePlayRatio = playingRatio;

	// �Đ��J�n����
	isPlay = true;
	playingRatio = 0.0f;

	// �X�V����
	pPrevAnimNode = pCurrentAnimNode;
	pCurrentAnimNode = GetNode(_animName);

	// �J�n����
	pCurrentAnimNode->Begin();

	if (!isInterp)	// ��Ԃ��Ȃ��Ȃ�
	{
		OnChangeAnimComplete();
		return;
	}

	// �J�ږ����Z�b�g����
	pCurTransArrow = _transitionArrow;
	float targetAnimRatio = pCurTransArrow->GetTargetRatio();
	float transitionTime = pCurTransArrow->GetTransitionTime();
	EaseKind ease = pCurTransArrow->GetEaseKind();

	// �Đ��J�n�����Ƃ��Ƀ^�[�Q�b�g�̊�������n�߂�
	playingRatio = pCurTransArrow->GetTargetRatio();

	// ��ԊJ�n
	switch (pCurTransArrow->GetInterpolateKind())
	{
	case AnimInterpolateKind::CrossFade:
		CrossFadeStart(changePlayRatio, targetAnimRatio, transitionTime, ease);
		break;

	case AnimInterpolateKind::Inertialization:
		InterpTransitionStart(targetAnimRatio, transitionTime);
		break;

	default:
		assert(!"�A�j���[�V�����Ԃ̕�Ԃ̎�ގw�肪����łȂ��ł�");
		break;
	}
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
	std::unique_ptr<SingleAnimationNode> pAnimNode
		= std::make_unique<SingleAnimationNode>(_nodeName);

	pAnimNode->SetAnimationData(_animName);
	pAnimationNodes[_nodeName] = std::move(pAnimNode);
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

void AnimationController::CreateTransitionArrow(const std::string& _fromNodeName, const std::string& _toNodeName, float _targetAnimRatio, float _transitionTime, std::function<bool()> _condition)
{
	AnimationNode_Base* fromNode = GetNode(_fromNodeName);
	if (!fromNode)
	{
		HASHI_DEBUG_LOG(_fromNodeName + " �J�ڌ��m�[�h��������܂���ł���");
		return;
	}

	AnimationNode_Base* toNode = GetNode(_toNodeName);
	if (!toNode)
	{
		HASHI_DEBUG_LOG(_toNodeName + " �J�ڐ�m�[�h��������܂���ł���");
		return;
	}

	if (fromNode == toNode) return;	// �������g�ɂȂ��Ă�����

	// ���쐬���Ēǉ�
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
		HASHI_DEBUG_LOG(_name + "��������܂���ł���");
		return nullptr;
	}


	return itr->second.get();
}

void AnimationController::ProgressPlayTime()
{
	if (isTransitioning) return;

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

void AnimationController::AnimatioUpdate()
{
	if (isTransitioning)	// �A�j���[�V�����J�ڒ��Ȃ�
	{
		switch (pCurTransArrow->GetInterpolateKind())
		{
		case AnimInterpolateKind::CrossFade:
			CrossFadeUpdate();
			break;

		case AnimInterpolateKind::Inertialization:
			InertInterpUpdate();
			break;
		}
	}
	else
		NormalUpdate();
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

void AnimationController::CacheUpdate()
{
	// �{�[���̃L���b�V���擾�̂��߃p�����[�^���擾����
	std::vector<BoneTransform> cacheTransforms;

	for (u_int b_i = 0; b_i < pBoneList->GetBoneCnt(); b_i++)
	{
		cacheTransforms.push_back(pBoneList->GetBone(b_i).GetAnimationTransform());
	}

	// ������ԗp�̃L���b�V�����X�V����
	pInertInterp->UpdateBoneCache(cacheTransforms);
}

void AnimationController::TranstionCheck()
{
	AnimTransitionArrow* pToAnimNode = pCurrentAnimNode->CheckTransition();

	if (!pToAnimNode) return;	// �J�ڂ��Ȃ��Ȃ�I���

	std::string nextName = pToAnimNode->GetToNode().GetNodeName();	// �J�ڐ�m�[�h��
	ChangeAnimation(nextName, pToAnimNode);
}

void AnimationController::OnChangeAnimComplete()
{
	HASHI_DEBUG_LOG(pCurrentAnimNode->GetNodeName() + "�ֈڍs");
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

void AnimationController::OnTransitionStart()
{
	isTransitioning = true;
}

void AnimationController::CrossFadeStart(float _changePlayRatio, float _targetAnimRatio, float _transitionTime, HashiTaku::EaseKind _easeKind)
{
	// �^�[�Q�b�g�̊����ƑJ�ڎ��Ԃ���J�n���̑J�ڐ�̊������v�Z
	float toAnimTime = pCurrentAnimNode->GetAnimationTime();	// �S�̎���
	float toTargetTime = toAnimTime * _targetAnimRatio;	//�ڕW�̎���
	float toStartTime = toTargetTime - _transitionTime;	//�@�J�n���̎���

	if (toStartTime < 0.0f)	// �J�n���Ԃ�0�b���O�Ȃ�
	{
		HASHI_DEBUG_LOG("�N���X�t�F�[�h�őJ�ڐ�̊J�n���Ԃ�0���O�ł�");
		toStartTime = 0.0f;

		// �J�ڐ�̊������ω�����̂ŕύX����
		playingRatio = _transitionTime / toAnimTime;
		playingRatio = std::min(playingRatio, 1.0f);
	}
	// �J�n���̊��������߂�
	float toStartRatio = toStartTime / toAnimTime;

	pCrossFadeInterp->Begin(*pPrevAnimNode, *pCurrentAnimNode, _transitionTime, _changePlayRatio, toStartRatio, _easeKind);

	OnTransitionStart();
}

void AnimationController::InterpTransitionStart(float _targetAnimRatio, float _transitionTime)
{
	u_int boneCnt = pBoneList->GetBoneCnt();
	std::vector<BoneTransform> requestPose(boneCnt);

	// ���̃A�j���[�V�����̎p�����擾����
	for (u_int b_i = 0; b_i < boneCnt; b_i++)
	{
		pCurrentAnimNode->GetAnimTransform(requestPose[b_i], b_i, _targetAnimRatio);
	}

	// ������Ԃ̏�������
	bool isSuccess = pInertInterp->Calculate(requestPose, _transitionTime);

	if (!isSuccess) return;	// ���s������

	OnTransitionStart();
}

void AnimationController::OnTransitionEnd()
{
	OnChangeAnimComplete();

	isTransitioning = false;	// �J�ڏI��
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
	IAnimInterpolate* interpolate = pCrossFadeInterp.get();
	if (pCurTransArrow->GetInterpolateKind() == AnimInterpolateKind::Inertialization)
		interpolate = pInertInterp.get();

	std::string text = pPrevAnimNode->GetNodeName();
	text += " ---> ";
	text += pCurrentAnimNode->GetNodeName();
	ImGui::Text(TO_UTF8(text));

	text = "�o�߁F" + std::to_string(interpolate->GetElapsedTime()) + "(s)";
	ImGui::Text(TO_UTF8(text));

	text = "�u�����h����" + std::to_string(interpolate->GetTransitionTime()) + "(s)";
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