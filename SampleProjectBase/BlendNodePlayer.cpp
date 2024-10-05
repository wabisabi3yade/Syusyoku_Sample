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

	// ブレンドの目標数値のポインタを取得する
	for (u_int a_i = 0; a_i < axisCnt; a_i++)
	{
		const std::string& paramName = pBlendNode->GetTargetParamName(a_i);
		axisPlayParameters[a_i].pTargetBlendValue = std::get_if<float>(pAnimParameters->GetValueAddress(paramName));
	}
}

void AnimBlendNodePlayer::Update()
{
	if (!IsCanUpdate()) return;

	// ブレンド値を移動する
	MoveCurrentBlend();

	// ブレンド値からアニメーションを更新する
	AnimationUpdate();
}

bool AnimBlendNodePlayer::IsCanUpdate()
{
	// 軸の数が0なら
	if (GetBlendAxisCnt() == 0) return false;

	return true;
}

void AnimBlendNodePlayer::MoveCurrentBlend()
{
	for (u_int a_i = 0; a_i < GetBlendAxisCnt(); a_i++)
	{
		AxisPlayParameter& playParam = axisPlayParameters[a_i];

		// 割合移動時間
		if (pBlendNode->GetBlendMoveTime(a_i) < Mathf::epsilon)
		{
			playParam.curBlendValue = *playParam.pTargetBlendValue;
			continue;
		}

		// 最後のターゲットから値が変わっているなら
		if (abs(*playParam.pTargetBlendValue - playParam.lastTargetBlendValue) > Mathf::epsilon)
		{
			// 値を更新、移動時間をリセット
			playParam.lastTargetBlendValue = *playParam.pTargetBlendValue;
			playParam.changeBlendValue = playParam.curBlendValue;
			playParam.curBlendMoveTime = 0.0f;
		}

		float blendMoveTime = pBlendNode->GetBlendMoveTime(a_i);
		HashiTaku::EaseKind blendMoveEase = pBlendNode->GetBlendMoveEasing(a_i);

		// 割合移動時間を進める
		playParam.curBlendMoveTime += MainApplication::DeltaTime();
		playParam.curBlendMoveTime = std::min(playParam.curBlendMoveTime, blendMoveTime);

		float easeValue = HashiTaku::Easing::EaseValue(playParam.curBlendMoveTime / blendMoveTime, blendMoveEase);
		float subRatio = *playParam.pTargetBlendValue - playParam.changeBlendValue;

		// イージングで割合を移動する
		playParam.curBlendValue = playParam.changeBlendValue + subRatio * easeValue;
	}
}

void AnimBlendNodePlayer::AnimationUpdate()
{
	// ブレンドするアニメーションデータを取得する
	std::vector<BlendAnimationNode::BlendingData> blendAnimations;
	
	if (GetBlendAxisCnt() == 1)	// 1軸
	{
		pBlendNode->FindBlendingDataSingleAxis(axisPlayParameters[0].curBlendValue, blendAnimations);
	}
	else // 2軸
	{
		DirectX::SimpleMath::Vector2 curBlendValues;
		curBlendValues = { axisPlayParameters[0].curBlendValue, axisPlayParameters[1].curBlendValue };
		pBlendNode->FindBlendPairTwoAxis(curBlendValues, blendAnimations);
	}

	u_int blendAnimCnt = static_cast<u_int>(blendAnimations.size());
	// アニメーションの数によってブレンド計算方法を変える
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
		HASHI_DEBUG_LOG("ブレンドのアニメーションの数が1,2,4以外");
		break;
	}
}

void AnimBlendNodePlayer::SingleAnimationUpdate(const std::vector<BlendAnimationNode::BlendingData>& _blendingData)
{
	float playingRatio = GetCurPlayRatio();

	const AnimationData& animData = *_blendingData[0].pAnimation;

	//ボーン数ループ
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
	// 比率
	float weightRatio = _blendingData[1].blendWeight;

	for (u_int b_i = 0; b_i < pBoneList->GetBoneCnt(); b_i++)
	{
		BoneTransform p_Transform;	// 前のアニメーション
		p_pAnimData.GetTransformByRatio(b_i, playingRatio, p_Transform);

		BoneTransform n_Transform;	// 後のアニメーション
		n_pAnimData.GetTransformByRatio(b_i, playingRatio, n_Transform);

		BoneTransform blendTransform;

		// 座標
		blendTransform.position = Vector3::Lerp(p_Transform.position, n_Transform.position, weightRatio);

		// 回転量
		blendTransform.rotation = Quaternion::Slerp(p_Transform.rotation, n_Transform.rotation, weightRatio);

		// トランスフォームをセット
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

		// 比率分アニメーションの座標・回転量をブレンドする
		for (u_int bl_i = 0; bl_i < 4; bl_i++)
		{
			boneTransform.position += _blendingData[bl_i].pAnimation->GetPositionByRatio(b_i, playingRatio) *
									  _blendingData[bl_i].blendWeight;
		}

		Quaternion quat12, quat34;	// クォータニオン1・2個目と3。4個目
		float weight12 = 0.0f, weight34 = 0.0f;	// ウェイト1・2個目と3。4個目
		
		// 1・2個目
		weight12 = _blendingData[0].blendWeight + _blendingData[1].blendWeight;
		quat12 = Quaternion::Slerp(
			_blendingData[0].pAnimation->GetQuaternionByRatio(b_i, playingRatio),
			_blendingData[1].pAnimation->GetQuaternionByRatio(b_i, playingRatio),
			_blendingData[1].blendWeight
		);
		quat12.Normalize();

		// 3・4個目
		weight34 = _blendingData[2].blendWeight + _blendingData[3].blendWeight;
		quat34 = Quaternion::Slerp(
			_blendingData[2].pAnimation->GetQuaternionByRatio(b_i, playingRatio),
			_blendingData[3].pAnimation->GetQuaternionByRatio(b_i, playingRatio),
			_blendingData[3].blendWeight
		);
		quat34.Normalize();

		// 1.2個目と3・4個目を球面線形補間でブレンドする
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
