#include "pch.h"
#include "BlendAnimationNode.h"
#include "AnimationParameters.h"

#include "AssetGetter.h"
#include "SkeletalMesh.h"

constexpr float DUPLICATE_PLUS(0.01f);	// �u�����h�l���d�������Ƃ��́{�l

using namespace DirectX::SimpleMath;
using namespace HashiTaku;

BlendAnimationNode::BlendAnimationNode(const AnimationParameters& _animParams, std::string _nodeName)
	: AnimationNode_Base(_nodeName, NodeType::Blend), pAnimParameters(&_animParams)
{
	// ������
	for (u_int a_i = 0; a_i < axisCnt; a_i++)
		targetBlendRatios[a_i] = nullptr;

	SetAnimationTime(1.0f);
}
void BlendAnimationNode::ImGuiPlaying()
{
	AnimationNode_Base::ImGuiPlaying();

	std::string text = "���݂̃u�����h: " + std::to_string(curBlendRatio);
	ImGui::Text(TO_UTF8(text));

	// �^�[�Q�b�g
	float target = targetBlendRatio;
	if (ImGui::SliderFloat("target", &target, 0.0f, 1.0f))
		SetTargetBlendRatio(target);
}

void BlendAnimationNode::Begin()
{
	AnimationNode_Base::Begin();

	targetBlendRatio = 0.0f;
	curBlendRatio = 0.0f;
	changeBlendRatio = 0.0f;
}

void BlendAnimationNode::Update(BoneList& _boneList)
{
	if (!IsCanUpdate()) return;

	// �u�����h�l���^�[�Q�b�g�ɋ߂Â���
	MoveCurBlend();

	// �A�j���[�V�������u�����h����
	AnimationUpdate(_boneList);
}

void BlendAnimationNode::SetAnimationData(const std::string& _animName)
{
	BlendData createData;

	AnimationData* pData = AssetGetter::GetAsset<AnimationData>(_animName);
	if (!pData) return;

	createData.pAnimation = pData;
	createData.ratioX = 0.0f;
	blendDatas.push_back(createData);
}

void BlendAnimationNode::SetTargetBlendRatio(float _ratio)
{
	// �����������Z�b�g����Ȃ珈�����Ȃ�
	if (abs(_ratio - targetBlendRatio) <= Mathf::epsilon)
		return;

	SetRatio(targetBlendRatio, _ratio);

	// �ړ����Ԃ����Z�b�g����
	curRatioSmoothTime = 0.0f;
	// �ύX���̋��߂�
	changeBlendRatio = curBlendRatio;
}

void BlendAnimationNode::SetAnimationRatio(float _ratio, const std::string& _animName)
{
	BlendData* findBlend = FindBlendData(_animName);
	// ������Ȃ�������
	if (findBlend == nullptr) return;

	SetRatio(findBlend->ratioX, _ratio);
}

nlohmann::json BlendAnimationNode::Save()
{
	auto data = AnimationNode_Base::Save();

	for (auto& b : blendDatas)	// �e�u�����h�f�[�^���Z�[�u
		data["blendData"].push_back(SaveBlendData(b));
	data["smoothTime"] = ratioSmoothTime;
	data["moveEase"] = ratioMoveEase;
	return data;
}

void BlendAnimationNode::Load(const nlohmann::json& _data)
{
	AnimationNode_Base::Load(_data);

	nlohmann::json blendSaveDatas;
	LoadJsonDataArray("blendData", blendSaveDatas, _data);
	for (auto& d : blendSaveDatas)	// �u�����h�f�[�^�����[�h����
		LoadBlendData(d);
}

void BlendAnimationNode::MoveCurBlend()
{
	// �����ړ�����
	if (ratioSmoothTime < Mathf::epsilon)
	{
		curBlendRatio = targetBlendRatio;
		return;
	}

	curRatioSmoothTime += MainApplication::DeltaTime();
	curRatioSmoothTime = std::min(curRatioSmoothTime, ratioSmoothTime);

	float easeValue = HashiTaku::Easing::EaseValue(curRatioSmoothTime / ratioSmoothTime, ratioMoveEase);

	float subRatio = targetBlendRatio - changeBlendRatio;

	// �C�[�W���O�Ŋ������ړ�����
	curBlendRatio = changeBlendRatio + subRatio * easeValue;
}

void BlendAnimationNode::AnimationUpdate(BoneList& _boneList)
{
	// �u�����h����2�̃A�j���[�V������T��
	BlendPair blendPair = FindBlendPair();

	if (blendPair.nextData == nullptr)	// �l������A�j���[�V�����������
		SingleUpdateAnimation(*blendPair.prevData, _boneList);
	else
		BlendUpdateAnimation(blendPair, _boneList);
}

bool BlendAnimationNode::IsCanUpdate()
{
	if (static_cast<u_int>(blendDatas.size()) == 0) return false;

	return true;
}

BlendAnimationNode::BlendPair BlendAnimationNode::FindBlendPair()
{
	BlendPair blendPair{};

	// �ȉ��̏����̂Ƃ��A�l������A�j���[�V�����͈����
	auto minItr = blendDatas.begin();
	auto maxItr = std::prev(blendDatas.end());

	if (curBlendRatio <= minItr->ratioX) // �ŏ��l��菬����
	{
		blendPair.prevData = &(*minItr);
		return blendPair;
	}
	else if (curBlendRatio >= maxItr->ratioX) // �ő�l���傫��
	{
		blendPair.prevData = &(*maxItr);
		return blendPair;
	}


	// ���݂̊������傫���������f�[�^�̒�����T��
	for (auto itr = blendDatas.begin(); itr != std::prev(blendDatas.end()); itr++)
	{
		auto nextItr = std::next(itr);
		if (curBlendRatio > nextItr->ratioX) continue;

		// ��������
		blendPair.prevData = &(*itr);
		blendPair.nextData = &(*nextItr);

		break;
	}
	return blendPair;
}

void BlendAnimationNode::SingleUpdateAnimation(BlendData& _animationData, BoneList& _boneList)
{
	const AnimationData& animData = *_animationData.pAnimation;

	float playingRatio = GetCurPlayRatio();

	//�{�[�������[�v
	for (unsigned int b_i = 0; b_i < _boneList.GetBoneCnt(); b_i++)
	{
		Bone& bone = _boneList.GetBone(b_i);

		BoneTransform transform;

		// �Đ����Ԃ���e�p�����[�^���擾
		// �X�P�[��
		transform.scale = animData.GetScaleByRatio(b_i, playingRatio);

		//�N�H�[�^�j�I��
		transform.rotation = animData.GetQuaternionByRatio(b_i, playingRatio);

		// ���W
		transform.position = animData.GetPositionByRatio(b_i, playingRatio);

		bone.SetAnimTransform(transform);
	}
}

void BlendAnimationNode::BlendUpdateAnimation(BlendPair& _blendPair, BoneList& _boneList)
{
	float deltaRatio = _blendPair.nextData->ratioX - _blendPair.prevData->ratioX;
	float playingRatio = GetCurPlayRatio();

	if (abs(deltaRatio) < Mathf::epsilon)	// �u�����h�l�ɍ����Ȃ��ꍇ
	{
		deltaRatio = Mathf::smallValue; // 0���Z���Ȃ��悤�ɂ���
	}

	// �A�j���[�V�����̏d�݊���
	float weightRatio = (curBlendRatio - _blendPair.prevData->ratioX) / deltaRatio;

	const AnimationData* p_pAnimData = _blendPair.prevData->pAnimation;
	const AnimationData* n_pAnimData = _blendPair.nextData->pAnimation;

	for (u_int b_i = 0; b_i < _boneList.GetBoneCnt(); b_i++)
	{
		BoneTransform p_Transform;	// �O�̃A�j���[�V����
		p_Transform = p_pAnimData->GetTransformByRatio(b_i, playingRatio);

		BoneTransform n_Transform;	// ��̃A�j���[�V����
		n_Transform = n_pAnimData->GetTransformByRatio(b_i, playingRatio);

		BoneTransform blendTransform;
		// ���W
		blendTransform.position = Vector3::Lerp(p_Transform.position, n_Transform.position, weightRatio);

		// �X�P�[��
		blendTransform.scale = Vector3::Lerp(p_Transform.scale, n_Transform.scale, weightRatio);

		// ��]��
		blendTransform.rotation = Quaternion::Slerp(p_Transform.rotation, n_Transform.rotation, weightRatio);

		// �g�����X�t�H�[�����Z�b�g
		Bone& bone = _boneList.GetBone(b_i);
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
	if (_setRatio < 0.0f || _setRatio > 1.0f)
		HASHI_DEBUG_LOG("0.0�`1.0�͈̔͂ő�����Ă�������");
#endif //  EDIT

	// �͈͓��ɐ���
	_ratio = std::min(_setRatio, 1.0f);
	_ratio = std::max(_ratio, 0.0f);
}

void BlendAnimationNode::SortBlendValue()
{
	blendDatas.sort(CompareBlendValue);
}

bool BlendAnimationNode::CompareBlendValue(const BlendData& _bd1, const BlendData& _bd2)
{
	return _bd1.ratioX > _bd2.ratioX;
}

void BlendAnimationNode::GetCurAnimTransform(BoneTransform& _outTransform, u_int _boneId) const
{
	GetAnimTransform(_outTransform, _boneId, GetCurPlayRatio());
}

void BlendAnimationNode::GetAnimTransform(BoneTransform& _outTransform, u_int _boneId, float _requestRatio) const
{
	// ���i�K�͍ŏ��̊�������
	const AnimationData* pAnimationData = blendDatas.begin()->pAnimation;
	_outTransform = pAnimationData->GetTransformByRatio(_boneId, _requestRatio);

}

void BlendAnimationNode::ImGuiSetting()
{
	AnimationNode_Base::ImGuiSetting();

	ImGui::DragFloat("RatioSmoothTime", &ratioSmoothTime, 0.01f, 0.0f, 10.0f);
	HashiTaku::Easing::ImGuiSelect(ratioMoveEase);
	for (auto& data : blendDatas)
	{
		std::string animName = data.pAnimation->GetAssetName();
		if (!ImGuiMethod::TreeNode(animName.c_str())) continue;
		ImGui::SliderFloat("Blend", &data.ratioX, 0.0f, 1.0f);
		ImGui::TreePop();
	}

	static std::string str;
	AssetGetter::ImGuiGetCombobox<AnimationData>("AddAnimation", str);
	if (ImGui::Button("Add Blend"))
		SetAnimationData(str);
}

void BlendAnimationNode::ImGuiChangeTargetParam()
{
	// float�^�ϐ��݂̂̃p�����[�^�����擾
	std::vector<const std::string*> pAnimParams;
	pAnimParameters->GetFloatNameList(pAnimParams);


}

nlohmann::json BlendAnimationNode::SaveBlendData(BlendData& _blendData)
{
	nlohmann::json saveData;
	saveData["animName"] = _blendData.pAnimation->GetAssetName();
	saveData["ratio"] = _blendData.ratioX;
	return saveData;
}

void BlendAnimationNode::LoadBlendData(const nlohmann::json& _blendData)
{
	BlendData loadBlend;

	loadBlend.pAnimation = LoadJsonAsset<AnimationData>("animName", _blendData);
	if (!loadBlend.pAnimation) return;

	LoadJsonFloat("ratio", loadBlend.ratioX, _blendData);
	blendDatas.push_back(std::move(loadBlend));
}

