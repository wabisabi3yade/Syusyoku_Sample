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

void AnimBlendNodePlayer::Update()
{
	if (!IsCanUpdate()) return;

	// �u�����h�l���ړ�����
	MoveCurrentBlend();

	// �u�����h�l����A�j���[�V�������X�V����
	AnimationUpdate();
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
		playParam.curBlendMoveTime += MainApplication::DeltaTime();
		playParam.curBlendMoveTime = std::min(playParam.curBlendMoveTime, blendMoveTime);

		float easeValue = HashiTaku::Easing::EaseValue(playParam.curBlendMoveTime / blendMoveTime, blendMoveEase);
		float subRatio = *playParam.pTargetBlendValue - playParam.changeBlendValue;

		// �C�[�W���O�Ŋ������ړ�����
		playParam.curBlendValue = playParam.changeBlendValue + subRatio * easeValue;
	}
}

void AnimBlendNodePlayer::AnimationUpdate()
{
	// �u�����h����A�j���[�V�����f�[�^���擾����
	std::vector<BlendAnimationNode::BlendingData> blendAnimations;
	
	if (GetBlendAxisCnt() == 1)	// 1��
	{
		pBlendNode->FindBlendingDataSingleAxis(axisPlayParameters[0].curBlendValue, blendAnimations);
	}
	else // 2��
	{
		DirectX::SimpleMath::Vector2 curBlendValues;
		curBlendValues = { axisPlayParameters[0].curBlendValue, axisPlayParameters[1].curBlendValue };
		pBlendNode->FindBlendPairTwoAxis(curBlendValues, blendAnimations);
	}

	u_int blendAnimCnt = static_cast<u_int>(blendAnimations.size());
	// �A�j���[�V�����̐��ɂ���ău�����h�v�Z���@��ς���
	switch (blendAnimCnt)
	{
	case 1:
		SingleAnimationUpdate(blendAnimations);
		break;

	case 2:
		TwoAnimationUpdate(blendAnimations);
		break;

	case 4:
		SquareAnimationUpdate(blendAnimations);
		break;

	default:
		HASHI_DEBUG_LOG("�u�����h�̃A�j���[�V�����̐���1,2,4�ȊO");
		break;
	}
}

void AnimBlendNodePlayer::SingleAnimationUpdate(const std::vector<BlendAnimationNode::BlendingData>& _blendingData)
{
	float playingRatio = GetCurPlayRatio();

	const AnimationData& animData = *_blendingData[0].pAnimation;

	//�{�[�������[�v
	for (unsigned int b_i = 0; b_i < pBoneList->GetBoneCnt(); b_i++)
	{
		Bone& bone = pBoneList->GetBone(b_i);
		BoneTransform transform;

		animData.GetTransformByRatio(b_i, playingRatio, transform);

		bone.SetAnimTransform(transform);
	}
}

void AnimBlendNodePlayer::TwoAnimationUpdate(const std::vector<BlendAnimationNode::BlendingData>& _blendingData)
{
	using namespace DirectX::SimpleMath;

	float playingRatio = GetCurPlayRatio();

	const AnimationData& p_pAnimData = *_blendingData[0].pAnimation;
	const AnimationData& n_pAnimData = *_blendingData[1].pAnimation;
	// �䗦
	float weightRatio = _blendingData[1].blendWeight;

	for (u_int b_i = 0; b_i < pBoneList->GetBoneCnt(); b_i++)
	{
		BoneTransform p_Transform;	// �O�̃A�j���[�V����
		p_pAnimData.GetTransformByRatio(b_i, playingRatio, p_Transform);

		BoneTransform n_Transform;	// ��̃A�j���[�V����
		n_pAnimData.GetTransformByRatio(b_i, playingRatio, n_Transform);

		BoneTransform blendTransform;

		// ���W
		blendTransform.position = Vector3::Lerp(p_Transform.position, n_Transform.position, weightRatio);

		// ��]��
		blendTransform.rotation = Quaternion::Slerp(p_Transform.rotation, n_Transform.rotation, weightRatio);

		// �g�����X�t�H�[�����Z�b�g
		Bone& bone = pBoneList->GetBone(b_i);
		bone.SetAnimTransform(blendTransform);
	}
}

void AnimBlendNodePlayer::SquareAnimationUpdate(const std::vector<BlendAnimationNode::BlendingData>& _blendingData)
{
	using namespace DirectX::SimpleMath;

	float playingRatio = GetCurPlayRatio();

	for (u_int b_i = 0; b_i < pBoneList->GetBoneCnt(); b_i++)
	{
		Bone& bone = pBoneList->GetBone(b_i);

		BoneTransform boneTransform;

		// �䗦���A�j���[�V�����̍��W�E��]�ʂ��u�����h����
		for (u_int bl_i = 0; bl_i < 4; bl_i++)
		{
			boneTransform.position += _blendingData[bl_i].pAnimation->GetPositionByRatio(b_i, playingRatio) *
									  _blendingData[bl_i].blendWeight;
		}

		Quaternion quat12, quat34;	// �N�H�[�^�j�I��1�E2�ڂ�3�B4��
		float weight12 = 0.0f, weight34 = 0.0f;	// �E�F�C�g1�E2�ڂ�3�B4��
		
		// 1�E2��
		weight12 = _blendingData[0].blendWeight + _blendingData[1].blendWeight;
		quat12 = Quaternion::Slerp(
			_blendingData[0].pAnimation->GetQuaternionByRatio(b_i, playingRatio),
			_blendingData[1].pAnimation->GetQuaternionByRatio(b_i, playingRatio),
			_blendingData[1].blendWeight
		);
		quat12.Normalize();

		// 3�E4��
		weight34 = _blendingData[2].blendWeight + _blendingData[3].blendWeight;
		quat34 = Quaternion::Slerp(
			_blendingData[2].pAnimation->GetQuaternionByRatio(b_i, playingRatio),
			_blendingData[3].pAnimation->GetQuaternionByRatio(b_i, playingRatio),
			_blendingData[3].blendWeight
		);
		quat34.Normalize();

		// 1.2�ڂ�3�E4�ڂ����ʐ��`��ԂŃu�����h����
		boneTransform.rotation = Quaternion::Slerp(quat12, quat34, weight34 / (weight12 + weight34));
		boneTransform.rotation.Normalize();

		bone.SetAnimTransform(boneTransform);
	}
}

u_int AnimBlendNodePlayer::GetBlendAxisCnt() const
{
	return static_cast<u_int>(axisPlayParameters.size());
}

DirectX::SimpleMath::Vector3 AnimBlendNodePlayer::GetRootMotionPos(float _ratio)
{
	return DirectX::SimpleMath::Vector3();
}

DirectX::SimpleMath::Quaternion AnimBlendNodePlayer::GetRootMotionRot(float _ratio)
{


	return DirectX::SimpleMath::Quaternion();
}
