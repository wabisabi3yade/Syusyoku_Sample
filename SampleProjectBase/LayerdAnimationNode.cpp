#include "pch.h"
#include "LayerdAnimationNode.h"
#include "AssetGetter.h"
#include "SkeletalMesh.h"

constexpr int NOT_SET_BONEID(-1);	// �{�[�����w�肵�Ă��Ȃ����̃{�[��ID

LayerdAnimationNode::LayerdAnimationNode(const BoneList& _boneList, const std::string& _nodeName) :
	AnimationNode_Base(_nodeName, NodeType::Layerd),
	pBoneList(&_boneList),
	pBaseAnimation(nullptr),
	pBlendAnimation(nullptr),
	beginBlendPlayFrame(0),
	beginBlendBoneId(NOT_SET_BONEID)
{
	blendSpeedCurve.SetCurveName("Blend Speed");
}

void LayerdAnimationNode::GetAnimTransform(std::vector<BoneTransform>& _outTransforms, float _baseRatio, float _blendRatio) const
{
	// �@�\�ł��Ă��Ȃ��Ȃ�
	if (!CanWarking()) return;

	// �x�[�X��
	u_int baseBoneCnt = static_cast<u_int>(baseBoneIds.size());
	for (u_int b_i = 0; b_i < baseBoneCnt; b_i++)
	{
		u_int uBoneId = static_cast<u_int>(baseBoneIds[b_i]);
		pBaseAnimation->GetTransformByRatio(uBoneId, _baseRatio, _outTransforms[uBoneId]);
	}

	// �u�����h��
	u_int blendBoneCnt = static_cast<u_int>(blendBoneIds.size());
	for (u_int b_i = 0; b_i < blendBoneCnt; b_i++)
	{
		u_int uBoneId = static_cast<u_int>(blendBoneIds[b_i]);
		pBlendAnimation->GetTransformByRatio(uBoneId, _blendRatio, _outTransforms[uBoneId]);
	}
}

float LayerdAnimationNode::GetBlendCurveSpeed(float _playRatio) const
{
	return blendSpeedCurve.GetValue(_playRatio);
}

float LayerdAnimationNode::GetBlendAnimationTime() const
{
	if (!pBlendAnimation) return 1.0f;

	return pBlendAnimation->GetAnimationTime();
}

float LayerdAnimationNode::GetBeginBlendPlayRatio() const
{
	if (!pBlendAnimation) return 0.0f;
	return static_cast<float>(beginBlendPlayFrame) / pBlendAnimation->GetAllAnimationFrame();
}

int LayerdAnimationNode::GetBeginBlendBoneId() const
{
	return beginBlendBoneId;
}

nlohmann::json LayerdAnimationNode::Save()
{
	auto data = AnimationNode_Base::Save();

	data["blendBeginFrame"] = beginBlendPlayFrame;
	data["blendCurve"] = blendSpeedCurve.Save();

	if (pBaseAnimation)
		data["baseAnim"] = pBaseAnimation->GetAssetName();
	if (pBlendAnimation)
		data["blendAnim"] = pBlendAnimation->GetAssetName();

	data["beginBoneId"] = beginBlendBoneId;

	return data;
}

void LayerdAnimationNode::Load(const nlohmann::json& _data)
{
	AnimationNode_Base::Load(_data);

	HashiTaku::LoadJsonUnsigned("blendBeginFrame", beginBlendPlayFrame, _data);
	nlohmann::json blendCurveData;
	if (HashiTaku::LoadJsonData("blendCurve", blendCurveData, _data))
	{
		blendSpeedCurve.Load(blendCurveData);
	}

	std::string loadStr;
	if (HashiTaku::LoadJsonString("baseAnim", loadStr, _data))
	{
		pBaseAnimation = AssetGetter::GetAsset<AnimationData>(loadStr);
		SetAnimationTime(pBaseAnimation->GetAnimationTime());
	}
	if (HashiTaku::LoadJsonString("blendAnim", loadStr, _data))
	{
		pBlendAnimation = AssetGetter::GetAsset<AnimationData>(loadStr);
		beginBlendPlayFrame = 0;
	}
	if (HashiTaku::LoadJsonInteger("beginBoneId", beginBlendBoneId, _data))
	{
		SetBlendBeginBone(beginBlendBoneId);
	}
}

bool LayerdAnimationNode::CanWarking() const
{
#ifdef EDIT
	// ��b�ƂȂ�A�j���[�V�������Ȃ�������@�\���Ȃ�
	if (!pBaseAnimation)
	{
		assert(!"��b�A�j���[�V�������Z�b�g���Ă�");
		return false;
	}

	if (!pBlendAnimation)
	{
		assert(!"�u�����h�A�j���[�V�������Z�b�g���Ă�");
		return false;
	}
#endif // EDIT

	return true;
}

void LayerdAnimationNode::SetBlendBeginBone(int _beginBoneId)
{
	// �{�[���̐����傫���@�܂��́@0�ȉ��Ȃ珈�����Ȃ�
	if (_beginBoneId >= static_cast<int>(pBoneList->GetBoneCnt()) ||
		_beginBoneId < 0) return;

	beginBlendBoneId = _beginBoneId;

	// �x�[�X���ƃu�����h���Ń{�[���𕪂��Ă���
	baseBoneIds.clear();
	blendBoneIds.clear();
	SeparateBoneGroup(*pBoneList->GetRootNode());
}

void LayerdAnimationNode::SeparateBoneGroup(const TreeNode& _treeNode)
{
	if (_treeNode.HasBone())
	{
		u_int boneId = _treeNode.GetBoneIdx();

		if (boneId == beginBlendBoneId) // �u�����h�J�n�{�[����������
		{
			// �u�����h�O���[�v���쐬����
			CreateBlendBoneGroup(_treeNode);
			return; // �ȍ~�̃{�[���͏������Ȃ�
		}

		baseBoneIds.push_back(boneId);	// �x�[�X�ɒǉ�
	}

	u_int childCnt = _treeNode.GetChildNum();
	// �ċA�ŕ����Ă���
	for (u_int c_i = 0; c_i < childCnt; c_i++)
		SeparateBoneGroup(*_treeNode.GetChild(c_i));
}

void LayerdAnimationNode::CreateBlendBoneGroup(const TreeNode& _blendBoneNode)
{
	if (_blendBoneNode.HasBone())
		blendBoneIds.push_back(_blendBoneNode.GetBoneIdx());	// �u�����h�ɒǉ�

	// �ċA�ŕ����Ă���
	u_int childCnt = _blendBoneNode.GetChildNum();
	for (u_int c_i = 0; c_i < childCnt; c_i++)
		CreateBlendBoneGroup(*_blendBoneNode.GetChild(c_i));
}

void LayerdAnimationNode::ImGuiDebug()
{
#ifdef EDIT
	AnimationNode_Base::ImGuiDebug();

	// �u�����h���̊J�n�t���[��
	u_int maxFrame = 0;
	if (pBlendAnimation) maxFrame = pBlendAnimation->GetAllAnimationFrame();
	ImGui::DragScalar("BlendBeginFrame", ImGuiDataType_U32, &beginBlendPlayFrame, 1, 0, &maxFrame);

	// �u�����h���̑��x�J�[�u
	blendSpeedCurve.ImGuiCall();

	// �A�j���[�V�������Z�b�g����
	static std::string str;
	AssetGetter::ImGuiGetCombobox<AnimationData>("Animation", str);
	if (ImGui::Button("Set Base"))
	{
		AnimationData* pGetData = AssetGetter::GetAsset<AnimationData>(str);
		if (pGetData)
		{
			pBaseAnimation = pGetData;
			SetAnimationTime(pBaseAnimation->GetAnimationTime());
		}
	}
	ImGui::SameLine();

	// �A�j���[�V�������\��
	std::string animName = "Null";
	if (pBaseAnimation)
		animName = pBaseAnimation->GetAssetName();
	ImGui::Text(animName.c_str());

	if (ImGui::Button("Set Blend"))
	{
		AnimationData* pGetData = AssetGetter::GetAsset<AnimationData>(str);
		if (pGetData) pBlendAnimation = pGetData;
	}
	ImGui::SameLine();
	// �A�j���[�V�������\��
	animName = "Null";
	if (pBlendAnimation)
		animName = pBlendAnimation->GetAssetName();
	ImGui::Text(animName.c_str());

	// �u�����h�J�n����{�[����Id���Z�b�g
	if (ImGui::DragInt("BlendBoneId", &beginBlendBoneId, 1, 0, (int)pBoneList->GetBoneCnt()))
	{
		SetBlendBeginBone(beginBlendBoneId);
	}
#endif // EDIT
}
