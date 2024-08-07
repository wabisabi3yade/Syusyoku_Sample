#include "pch.h"
#include "BlendAnimationNode.h"

#include "AssetGetter.h"

#include "SkeletalMesh.h"

constexpr float DUPLICATE_PLUS(0.01f);	// �u�����h�l���d�������Ƃ��́{�l

using namespace DirectX::SimpleMath;

BlendAnimationNode::BlendAnimationNode(std::string _nodeName) : AnimationNode_Base(_nodeName, NodeType::Blend), curBlendRatio(0.0f), targetBlendRatio(0.0f), ratioMoveTime(0.0f)
{
	SetAnimationTime(1.0f);
}

void BlendAnimationNode::ImGuiPlaying()
{
	ImGui::Begin(TO_UTF8("�u�����h�A�j���[�V����"));

	AnimationNode_Base::ImGuiPlaying();

	std::string text = "���݂̃u�����h: " + std::to_string(curBlendRatio);
	ImGui::Text(TO_UTF8(text));

	ImGui::DragFloat(TO_UTF8("�^�[�Q�b�g"), &targetBlendRatio, 0.01f, 0.0f, 1.0f);

	for (auto& data : blendDatas)
	{
		std::string animName = data.pAnimation->GetAssetName();
		if (!ImGui::TreeNode(animName.c_str())) continue;
;
		ImGui::DragFloat(TO_UTF8("�u�����h"), &data.ratio, 0.01f, 0.0f, 1.0f);

		ImGui::TreePop();
	}

	ImGui::End();
}

void BlendAnimationNode::ImGuiSetting()
{
	if (!ImGui::TreeNode("Import")) return;

	static char animName[256] = "";
	ImGui::InputText("AnimName", animName, 256);

	if (ImGui::Button("Set"))
	{
		AnimationData* a = AssetGetter::GetAsset<AnimationData>(animName);
		if (a)
		{
			SetAnimationData(*a);
		}
	}

	ImGui::TreePop();
}

void BlendAnimationNode::Update(float _playingTime, BoneList& _boneList)
{
	// �u�����h�l���^�[�Q�b�g�ɋ߂Â���
	MoveCurBlend();
	
	// �A�j���[�V�������u�����h����
	AnimationUpdate(_playingTime, _boneList);
}

void BlendAnimationNode::SetAnimationData(AnimationData& _animData)
{
	BlendData createData;
	createData.pAnimation = &_animData;
	createData.ratio = 0.0f;

	blendDatas.push_back(createData);
}

void BlendAnimationNode::SetTargetBlendRatio(float _ratio)
{
	SetRatio(targetBlendRatio, _ratio);
}

void BlendAnimationNode::SetAnimationRatio(float _ratio, const std::string& _animName)
{
	BlendData* findBlend = FindBlendData(_animName);
	// ������Ȃ�������
	if (findBlend == nullptr) return;

	SetRatio(findBlend->ratio, _ratio);
}

void BlendAnimationNode::MoveCurBlend()
{
	// �C�[�W���O�Ȃǂ�

	curBlendRatio = targetBlendRatio;
}

void BlendAnimationNode::AnimationUpdate(float _playingRatio, BoneList& _boneList)
{
	// �u�����h����2�̃A�j���[�V������T��
	BlendPair blendPair = FindBlendPair();

	if (blendPair.nextData == nullptr)	// �l������A�j���[�V�����������
		SingleUpdateAnimation(*blendPair.prevData, _playingRatio, _boneList);
	else
		BlendUpdateAnimation(blendPair, _playingRatio, _boneList);
}

BlendAnimationNode::BlendPair BlendAnimationNode::FindBlendPair()
{
	BlendPair blendPair{};

	// �ȉ��̏����̂Ƃ��A�l������A�j���[�V�����͈����
	auto minItr = blendDatas.begin();
	auto maxItr = std::prev(blendDatas.end());

	if (curBlendRatio <= minItr->ratio) // �ŏ��l��菬����
	{
		blendPair.prevData = &(*minItr);
		return blendPair;
	}
	else if (curBlendRatio >= maxItr->ratio) // �ő�l���傫��
	{
		blendPair.prevData = &(*maxItr);
		return blendPair;
	}


	// ���݂̊������傫���������f�[�^�̒�����T��
	for (auto itr = blendDatas.begin(); itr != std::prev(blendDatas.end()); itr++)
	{
		auto nextItr = std::next(itr);
		if (curBlendRatio > nextItr->ratio) continue;

		// ��������
		blendPair.prevData = &(*itr);
		blendPair.nextData = &(*nextItr);

		break;
	}
	return blendPair;
}

void BlendAnimationNode::SingleUpdateAnimation(BlendData& _animationData, float _playingRatio, BoneList& _boneList)
{
	const AnimationData& animData = *_animationData.pAnimation;

	//�{�[�������[�v
	for (unsigned int b_i = 0; b_i < _boneList.GetBoneCnt(); b_i++)
	{
		Bone& bone = _boneList.GetBone(b_i);

		BoneTransform transform;

		// �Đ����Ԃ���e�p�����[�^���擾
		// �X�P�[��
		transform.scale = animData.GetScale(b_i, _playingRatio);

		//�N�H�[�^�j�I��
		transform.rotation = animData.GetQuaternion(b_i, _playingRatio);

		// ���W
		transform.position = animData.GetPosition(b_i, _playingRatio);

		bone.SetAnimTransform(transform);
	}
}

void BlendAnimationNode::BlendUpdateAnimation(BlendPair& _blendPair, float _playingRatio, BoneList& _boneList)
{
	float deltaRatio = _blendPair.nextData->ratio - _blendPair.prevData->ratio;

	if (deltaRatio == 0.0f)	// �u�����h�l�ɍ����Ȃ��ꍇ
	{
		deltaRatio += Mathf::smallValue; // 0���Z���Ȃ��悤�ɂ���
	}

	// �A�j���[�V�����̏d�݊���
	float weightRatio = (curBlendRatio - _blendPair.prevData->ratio) / deltaRatio;

	const AnimationData* p_pAnimData = _blendPair.prevData->pAnimation;
	const AnimationData* n_pAnimData = _blendPair.nextData->pAnimation;

	for (u_int b_i = 0; b_i < _boneList.GetBoneCnt(); b_i++)
	{
		BoneTransform p_Transform;	// �O�̃A�j���[�V����
		p_Transform = p_pAnimData->GetTransform(b_i, _playingRatio);

		BoneTransform n_Transform;	// ��̃A�j���[�V����
		n_Transform = n_pAnimData->GetTransform(b_i, _playingRatio);

		BoneTransform blendTransform;
		// ���W
		blendTransform.position = Vector3::Lerp(p_Transform.position, n_Transform.position, weightRatio);

		// �X�P�[��
		blendTransform.scale = Vector3::Lerp(p_Transform.scale, n_Transform.scale, weightRatio);

		// ��]��
		blendTransform.rotation = Quaternion::Slerp(p_Transform.rotation, n_Transform.rotation, weightRatio);

		// �g�����X�t�H�[�����Z�b�g
		Bone& bone =  _boneList.GetBone(b_i);
		bone.SetAnimTransform(blendTransform);
	}
}

BlendAnimationNode::BlendData* BlendAnimationNode::FindBlendData(const std::string& _animName)
{
	// ���O����T��
	auto itr = std::find_if(blendDatas.begin(), blendDatas.end(), [&](BlendData& data)
		{
			return data.pAnimation->GetAssetName() == _animName;
		});

	if (itr == blendDatas.end())
	{
		HASHI_DEBUG_LOG("�A�j���[�V��������������܂���ł���");
		return nullptr;
	}

	return &(*itr);
}

void BlendAnimationNode::SetRatio(float& _ratio, float _setRatio)
{
#ifdef EDIT
	if (_ratio < 0.0f || _ratio > 1.0f)
		HASHI_DEBUG_LOG("0.0�`1.0�͈̔͂ő�����Ă�������");
#endif //  EDIT

	// �͈͓��ɐ���
	_ratio = std::min(_ratio, 1.0f);
	_ratio = std::max(_ratio, 0.0f);
}

void BlendAnimationNode::SortBlendValue()
{
	blendDatas.sort(CompareBlendValue);
}

bool BlendAnimationNode::CompareBlendValue(const BlendData& _bd1, const BlendData& _bd2)
{
	return _bd1.ratio > _bd2.ratio;
}
