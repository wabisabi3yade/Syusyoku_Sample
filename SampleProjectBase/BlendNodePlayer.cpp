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

void AnimBlendNodePlayer::Update(std::vector<BoneTransform>& _outTransforms)
{
	if (!IsCanUpdate()) return;

	// ブレンド値を移動する
	MoveCurrentBlend();

	// ブレンド値からアニメーションを更新する
	AnimationUpdate(_outTransforms);
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

void AnimBlendNodePlayer::AnimationUpdate(std::vector<BoneTransform>& _outTransforms)
{
	blendingAnimData.clear();

	// ブレンドするアニメーションデータを取得する
	if (GetBlendAxisCnt() == 1)	// 1軸
	{
		pBlendNode->FindBlendingDataSingleAxis(axisPlayParameters[0].curBlendValue, blendingAnimData);
	}
	else // 2軸
	{
		DirectX::SimpleMath::Vector2 curBlendValues;
		curBlendValues = { axisPlayParameters[0].curBlendValue, axisPlayParameters[1].curBlendValue };
		pBlendNode->FindBlendPairTwoAxis(curBlendValues, blendingAnimData);
	}

	u_int blendAnimCnt = static_cast<u_int>(blendingAnimData.size());

	_outTransforms.resize(pBoneList->GetBoneCnt());
	// アニメーションの数によってブレンド計算方法を変える
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
		HASHI_DEBUG_LOG("ブレンドのアニメーションの数が1,2,4以外");
		break;
	}
}

void AnimBlendNodePlayer::SingleAnimationUpdate(std::vector<BoneTransform>& _outTransforms)
{
	float playingRatio = GetAnimationRatio();

	const AnimationData& animData = *blendingAnimData[0].pAnimation;

	//ボーン数ループ
	for (unsigned int b_i = 0; b_i < pBoneList->GetBoneCnt(); b_i++)
	{
		animData.GetTransformByRatio(b_i, playingRatio, _outTransforms[b_i]);
	}
}

void AnimBlendNodePlayer::TwoAnimationUpdate(std::vector<BoneTransform>& _outTransforms)
{
	using namespace DirectX::SimpleMath;

	float playingRatio = GetAnimationRatio();

	const AnimationData& p_pAnimData = *blendingAnimData[0].pAnimation;
	const AnimationData& n_pAnimData = *blendingAnimData[1].pAnimation;
	// 比率
	float weightRatio = blendingAnimData[1].blendWeight;

	for (u_int b_i = 0; b_i < pBoneList->GetBoneCnt(); b_i++)
	{
		BoneTransform p_Transform;	// 前のアニメーション
		p_pAnimData.GetTransformByRatio(b_i, playingRatio, p_Transform);

		BoneTransform n_Transform;	// 後のアニメーション
		n_pAnimData.GetTransformByRatio(b_i, playingRatio, n_Transform);

		// 座標
		_outTransforms[b_i].position = Vector3::Lerp(p_Transform.position, n_Transform.position, weightRatio);

		// 回転量
		_outTransforms[b_i].rotation = Quaternion::Slerp(p_Transform.rotation, n_Transform.rotation, weightRatio);
	}
}

void AnimBlendNodePlayer::SquareAnimationUpdate(std::vector<BoneTransform>& _outTransforms)
{
	using namespace DirectX::SimpleMath;

	float playingRatio = GetAnimationRatio();

	for (u_int b_i = 0; b_i < pBoneList->GetBoneCnt(); b_i++)
	{
		// 比率分アニメーションの座標・回転量をブレンドする
		for (u_int bl_i = 0; bl_i < 4; bl_i++)
		{
			_outTransforms[b_i].position += blendingAnimData[bl_i].pAnimation->GetPositionByRatio(b_i, playingRatio) *
									  blendingAnimData[bl_i].blendWeight;
		}

		Quaternion quat12, quat34;	// クォータニオン1・2個目と3。4個目
		float weight12 = 0.0f, weight34 = 0.0f;	// ウェイト1・2個目と3。4個目
		
		// 1・2個目
		weight12 = blendingAnimData[0].blendWeight + blendingAnimData[1].blendWeight;
		quat12 = Quaternion::Slerp(
			blendingAnimData[0].pAnimation->GetQuaternionByRatio(b_i, playingRatio),
			blendingAnimData[1].pAnimation->GetQuaternionByRatio(b_i, playingRatio),
			blendingAnimData[1].blendWeight / weight12
		);
		quat12.Normalize();

		// 3・4個目
		weight34 = blendingAnimData[2].blendWeight + blendingAnimData[3].blendWeight;
		quat34 = Quaternion::Slerp(
			blendingAnimData[2].pAnimation->GetQuaternionByRatio(b_i, playingRatio),
			blendingAnimData[3].pAnimation->GetQuaternionByRatio(b_i, playingRatio),
			blendingAnimData[3].blendWeight / weight34
		);
		quat34.Normalize();

		// 1.2個目と3・4個目を球面線形補間でブレンドする
		_outTransforms[b_i].rotation = Quaternion::Slerp(quat12, quat34, weight34 / (weight12 + weight34));
		_outTransforms[b_i].rotation.Normalize();
	}
}

u_int AnimBlendNodePlayer::GetBlendAxisCnt() const
{
	return static_cast<u_int>(axisPlayParameters.size());
}

void AnimBlendNodePlayer::CalcRootMotionPosSpeed(float _controllerSpeed)
{
	using namespace DirectX::SimpleMath;
	pBlendNode->CalcRootMotionSpeed(blendingAnimData, rootMotionPosSpeedPerSec);
	rootMotionPosSpeedPerSec *= _controllerSpeed /** GetNodePlaySpeed()*/ * pBoneList->GetLoadScale();;
}

DirectX::SimpleMath::Vector3 AnimBlendNodePlayer::GetRootMotionPos(float _ratio, bool _isLoadScaling) const
{
	using namespace DirectX::SimpleMath;
	Vector3 rootMotionPos;
	const BlendAnimationNode& blendNode = static_cast<const BlendAnimationNode&>(*pPlayAnimNode);
	blendNode.GetRootMotionPos(_ratio, blendingAnimData, rootMotionPos);

	// ロード時の回転量と、スケールを掛ける
	if (_isLoadScaling)
		ApplyLoadTransform(rootMotionPos);

	return rootMotionPos;
}

DirectX::SimpleMath::Quaternion AnimBlendNodePlayer::GetRootMotionRot(float _ratio, bool _isLoadScaling) const
{
	return DirectX::SimpleMath::Quaternion();
}

void AnimBlendNodePlayer::ImGuiSetting()
{
	AnimNodePlayer_Base::ImGuiSetting();

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
