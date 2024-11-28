#include "pch.h"
#include "BlendNodePlayer.h"
#include "BlendAnimationNode.h"
#include "AnimationParameters.h"

AnimBlendNodePlayer::AnimBlendNodePlayer(const AnimationParameters& _animParameters, const AnimationNode_Base& _playNode, BoneList& _boneList, Transform& _transform)
	:AnimNodePlayer_Base(_playNode, _boneList, _transform), pAnimParameters(&_animParameters)
{
	Init();
}

void AnimBlendNodePlayer::Init()
{
	pBlendNode = static_cast<const BlendAnimationNode*>(pPlayAnimNode);

	u_int axisCnt = pBlendNode->GetAxisCnt();
	axisPlayParameters.resize(axisCnt);

	// �u�����h�̖ڕW���l�̃|�C���^���擾����
	for (u_int a_i = 0; a_i < axisCnt; a_i++)
	{
		const std::string& paramName = pBlendNode->GetTargetParamName(a_i);
		axisPlayParameters[a_i].pTargetBlendValue = std::get_if<float>(pAnimParameters->GetValueAddress(paramName));
	}
}

void AnimBlendNodePlayer::Update(std::vector<BoneTransform>& _outTransforms)
{
	if (!IsCanUpdate()) return;

	// �u�����h�l���ړ�����
	MoveCurrentBlend();

	// �u�����h�l����A�j���[�V�������X�V����
	AnimationUpdate(_outTransforms);
}

bool AnimBlendNodePlayer::IsCanUpdate()
{
	// ���̐���0�Ȃ�
	if (GetBlendAxisCnt() == 0) return false;

	return true;
}

void AnimBlendNodePlayer::MoveCurrentBlend()
{
	for (u_int a_i = 0; a_i < GetBlendAxisCnt(); a_i++)
	{
		AxisPlayParameter& playParam = axisPlayParameters[a_i];

		// �����ړ�����
		if (pBlendNode->GetBlendMoveTime(a_i) < Mathf::epsilon)
		{
			playParam.curBlendValue = *playParam.pTargetBlendValue;
			continue;
		}

		// �Ō�̃^�[�Q�b�g����l���ς���Ă���Ȃ�
		if (abs(*playParam.pTargetBlendValue - playParam.lastTargetBlendValue) > Mathf::epsilon)
		{
			// �l���X�V�A�ړ����Ԃ����Z�b�g
			playParam.lastTargetBlendValue = *playParam.pTargetBlendValue;
			playParam.changeBlendValue = playParam.curBlendValue;
			playParam.curBlendMoveTime = 0.0f;
		}

		float blendMoveTime = pBlendNode->GetBlendMoveTime(a_i);
		HashiTaku::EaseKind blendMoveEase = pBlendNode->GetBlendMoveEasing(a_i);

		// �����ړ����Ԃ�i�߂�
		playParam.curBlendMoveTime += deltaTime;
		playParam.curBlendMoveTime = std::min(playParam.curBlendMoveTime, blendMoveTime);

		float easeValue = HashiTaku::Easing::EaseValue(playParam.curBlendMoveTime / blendMoveTime, blendMoveEase);
		float subRatio = *playParam.pTargetBlendValue - playParam.changeBlendValue;

		// �C�[�W���O�Ŋ������ړ�����
		playParam.curBlendValue = playParam.changeBlendValue + subRatio * easeValue;
	}
}

void AnimBlendNodePlayer::AnimationUpdate(std::vector<BoneTransform>& _outTransforms)
{
	blendingAnimData.clear();

	// �u�����h����A�j���[�V�����f�[�^���擾����
	if (GetBlendAxisCnt() == 1)	// 1��
	{
		pBlendNode->FindBlendingDataSingleAxis(axisPlayParameters[0].curBlendValue, blendingAnimData);
	}
	else // 2��
	{
		DirectX::SimpleMath::Vector2 curBlendValues;
		curBlendValues = { axisPlayParameters[0].curBlendValue, axisPlayParameters[1].curBlendValue };
		pBlendNode->FindBlendPairTwoAxis(curBlendValues, blendingAnimData);
	}

	// �u�����h���Ȃ�������
	if (!pBlendNode->GetIsUseBlend())
	{
		// ��ԃE�F�C�g���傫�����̂������c��
		blendingAnimData.erase(blendingAnimData.begin() + 1, blendingAnimData.end());
		blendingAnimData[0].blendWeight = 1.0f;
	}

	u_int blendAnimCnt = static_cast<u_int>(blendingAnimData.size());
	_outTransforms.resize(pAssetBoneList->GetBoneCnt());
	// �A�j���[�V�����̐��ɂ���ău�����h�v�Z���@��ς���
	switch (blendAnimCnt)
	{
	case 1:
		SingleAnimationUpdate(_outTransforms);
		break;

	case 2:
		TwoAnimationUpdate(_outTransforms);
		break;

	case 4:
		SquareAnimationUpdate(_outTransforms);
		break;

	default:
		HASHI_DEBUG_LOG("�u�����h�̃A�j���[�V�����̐���1,2,4�ȊO");
		break;
	}
}

void AnimBlendNodePlayer::SingleAnimationUpdate(std::vector<BoneTransform>& _outTransforms)
{
	float playingRatio = GetCurPlayRatio();

	const AnimationData& animData = *blendingAnimData[0].pAnimation;

	//�{�[�������[�v
	for (unsigned int b_i = 0; b_i < pAssetBoneList->GetBoneCnt(); b_i++)
	{
		animData.GetTransformByRatio(b_i, playingRatio, _outTransforms[b_i]);
	}
}

void AnimBlendNodePlayer::TwoAnimationUpdate(std::vector<BoneTransform>& _outTransforms)
{
	using namespace DirectX::SimpleMath;

	float playingRatio = GetCurPlayRatio();

	const AnimationData& p_pAnimData = *blendingAnimData[0].pAnimation;
	const AnimationData& n_pAnimData = *blendingAnimData[1].pAnimation;
	// �䗦
	float weightRatio = blendingAnimData[1].blendWeight;

	for (u_int b_i = 0; b_i < pAssetBoneList->GetBoneCnt(); b_i++)
	{
		BoneTransform p_Transform;	// �O�̃A�j���[�V����
		p_pAnimData.GetTransformByRatio(b_i, playingRatio, p_Transform);

		BoneTransform n_Transform;	// ��̃A�j���[�V����
		n_pAnimData.GetTransformByRatio(b_i, playingRatio, n_Transform);

		// ���W
		_outTransforms[b_i].position = Vector3::Lerp(p_Transform.position, n_Transform.position, weightRatio);

		// ��]��
		_outTransforms[b_i].rotation = Quaternion::Slerp(p_Transform.rotation, n_Transform.rotation, weightRatio);
	}
}

void AnimBlendNodePlayer::SquareAnimationUpdate(std::vector<BoneTransform>& _outTransforms)
{
	using namespace DirectX::SimpleMath;

	float playingRatio = GetCurPlayRatio();

	for (u_int b_i = 0; b_i < pAssetBoneList->GetBoneCnt(); b_i++)
	{
		// �䗦���A�j���[�V�����̍��W�E��]�ʂ��u�����h����
		for (u_int bl_i = 0; bl_i < 4; bl_i++)
		{
			_outTransforms[b_i].position += blendingAnimData[bl_i].pAnimation->GetPositionByRatio(b_i, playingRatio) *
				blendingAnimData[bl_i].blendWeight;
		}

		Quaternion quat12, quat34;	// �N�H�[�^�j�I��1�E2�ڂ�3�B4��
		float weight12 = 0.0f, weight34 = 0.0f;	// �E�F�C�g1�E2�ڂ�3�B4��

		// 1�E2��
		weight12 = blendingAnimData[0].blendWeight + blendingAnimData[1].blendWeight;
		quat12 = Quaternion::Slerp(
			blendingAnimData[0].pAnimation->GetQuaternionByRatio(b_i, playingRatio),
			blendingAnimData[1].pAnimation->GetQuaternionByRatio(b_i, playingRatio),
			blendingAnimData[1].blendWeight / weight12
		);
		quat12.Normalize();

		// 3�E4��
		weight34 = blendingAnimData[2].blendWeight + blendingAnimData[3].blendWeight;
		quat34 = Quaternion::Slerp(
			blendingAnimData[2].pAnimation->GetQuaternionByRatio(b_i, playingRatio),
			blendingAnimData[3].pAnimation->GetQuaternionByRatio(b_i, playingRatio),
			blendingAnimData[3].blendWeight / weight34
		);
		quat34.Normalize();

		// 1.2�ڂ�3�E4�ڂ����ʐ��`��ԂŃu�����h����
		float t = weight34 / (weight12 + weight34);
		t = std::clamp(t, 0.0f, 1.0f);
		_outTransforms[b_i].rotation = Quaternion::Slerp(quat12, quat34, t);
		_outTransforms[b_i].rotation.Normalize();
	}
}

u_int AnimBlendNodePlayer::GetBlendAxisCnt() const
{
	return static_cast<u_int>(axisPlayParameters.size());
}

void AnimBlendNodePlayer::CalcRootMotionPosSpeed()
{
	using namespace DirectX::SimpleMath;

	pBlendNode->CalcRootMotionSpeed(blendingAnimData, rootMotionPosSpeedPerSec);

	// �m�[�h�Đ����x�͓K�p���Ȃ�
	rootMotionPosSpeedPerSec *= pAssetBoneList->GetLoadScale() *
		allPlaySpeed /
		GetNodePlaySpeed();
}

DirectX::SimpleMath::Vector3 AnimBlendNodePlayer::GetRootMotionPos(float _ratio, bool _isLoadScaling) const
{
	using namespace DirectX::SimpleMath;
	Vector3 rootMotionPos;
	const BlendAnimationNode& blendNode = static_cast<const BlendAnimationNode&>(*pPlayAnimNode);
	blendNode.GetRootMotionPos(_ratio, blendingAnimData, rootMotionPos);

	// ���[�h���̉�]�ʂƁA�X�P�[�����|����
	if (_isLoadScaling)
		ApplyLoadTransform(rootMotionPos);

	return rootMotionPos;
}

DirectX::SimpleMath::Quaternion AnimBlendNodePlayer::GetRootMotionRot(float _ratio, bool _isLoadScaling) const
{
	return DirectX::SimpleMath::Quaternion();
}

void AnimBlendNodePlayer::ImGuiDebug()
{
	AnimNodePlayer_Base::ImGuiDebug();

	ImGui::Text("Bllending");

	u_int blendCnt = static_cast<u_int>(blendingAnimData.size());
	for (u_int b_i = 0; b_i < blendCnt; b_i++)
	{
		std::string text = std::to_string(b_i + 1) + ":" + blendingAnimData[b_i].pAnimation->GetAssetName();
		ImGui::Text(text.c_str()); ImGui::SameLine();
		ImGui::Text("\t%f", blendingAnimData[b_i].blendWeight);
	}

	if (ImGuiMethod::TreeNode("Axis X"))
	{
		ImGuiAxisParameer(axisPlayParameters[0]);
		ImGui::TreePop();
	}

	if (GetBlendAxisCnt() < 2) return;

	if (ImGuiMethod::TreeNode("Axis Y"))
	{
		ImGuiAxisParameer(axisPlayParameters[1]);
		ImGui::TreePop();
	}
}

void AnimBlendNodePlayer::ImGuiAxisParameer(const AxisPlayParameter& _axis)
{
	ImGui::Text("target:%f", *_axis.pTargetBlendValue);
	ImGui::Text("curBlendVal:%f", _axis.curBlendValue);
	ImGui::Text("curBlendTime:%f", _axis.curBlendMoveTime);
}
