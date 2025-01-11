#include "pch.h"
#include "AnimLayerdNodePlayer.h"
#include "LayerdAnimationNode.h"

namespace HashiTaku
{
	AnimLayerdNodePlayer::AnimLayerdNodePlayer(const AnimationNode_Base& _playNode, BoneList& _boneList, Transform& _transform) :
		AnimNodePlayer_Base(_playNode, _boneList, _transform),
		curBlendPlayRatio(0.0f)
	{
		const LayerdAnimationNode& layerNode = static_cast<const LayerdAnimationNode&>(_playNode);

		// ノードで決められた再生割合から始める
		curBlendPlayRatio = layerNode.GetBeginBlendPlayRatio();
	}

	void AnimLayerdNodePlayer::ProgressPlayRatio(float _controllerSpeed)
	{
		const LayerdAnimationNode& layerdNode = static_cast<const LayerdAnimationNode&>(*pPlayAnimNode);

		AnimNodePlayer_Base::ProgressPlayRatio(_controllerSpeed);

		float speed =
			GetPlayerSpeed() *
			layerdNode.GetBlendCurveSpeed(curBlendPlayRatio) *
			pPlayAnimNode->GetPlaySpeedTimes();

		// 0徐算防止
		float animTime = std::max(layerdNode.GetBlendAnimationTime(), Mathf::epsilon);

		// 進める
		curBlendPlayRatio += speed * deltaTime / animTime;
		if (curBlendPlayRatio > 1.0f)
			curBlendPlayRatio -= 1.0f;
	}

	void AnimLayerdNodePlayer::Update(std::vector<BoneTransform>& _outTransforms)
	{
		//ボーン数ループ
		const LayerdAnimationNode& layerdNode = static_cast<const LayerdAnimationNode&>(*pPlayAnimNode);

		// 再生時間から各パラメータを取得
		layerdNode.GetAnimTransform(_outTransforms, GetCurPlayRatio(), curBlendPlayRatio);
	}

	void AnimLayerdNodePlayer::CalcRootMotionPosSpeed()
	{
		// コントローラーの
		const LayerdAnimationNode& layerdNode = static_cast<const  LayerdAnimationNode&>(*pPlayAnimNode);

		float nodePlaySpeed = std::max(GetNodePlaySpeed(), Mathf::epsilon);

		rootMotionPosSpeedPerSec =
			layerdNode.GetRootMotionPerSpeed() *
			pObjectTransform->GetScale() *
			pAssetBoneList->GetLoadScale() *
			(allPlaySpeed / nodePlaySpeed);
	}

	DXSimp::Vector3 AnimLayerdNodePlayer::GetRootMotionPos(float _ratio, bool _isWorldScaling) const
	{
		const LayerdAnimationNode& layerdNode = static_cast<const  LayerdAnimationNode&>(*pPlayAnimNode);

		DXSimp::Vector3 rootMotionPos = layerdNode.GetRootMotionPos(_ratio);

		// ロード時の回転量と、スケールを掛ける
		if (_isWorldScaling)
			ApplyLoadTransform(rootMotionPos);

		return rootMotionPos;
	}

	void AnimLayerdNodePlayer::ImGuiDebug()
	{
		AnimNodePlayer_Base::ImGuiDebug();

		const LayerdAnimationNode& layerNode =
			static_cast<const LayerdAnimationNode&>(*pPlayAnimNode);

		// キーフレームで移動(ベース)
		int frame = static_cast<int>(GetCurPlayRatio() * layerNode.GetAllKeyFrame());
		if (ImGui::SliderInt("BaseFrame", &frame, 0, layerNode.GetAllKeyFrame()))
		{
			SetCurPlayRatio(static_cast<float>(frame) / layerNode.GetAllKeyFrame());
		}

		ImGuiMethod::LineSpaceSmall();
		ImGui::PushID("Blend");
		ImGui::Text("Blend");
		ImGui::SliderFloat("Ratio", &curBlendPlayRatio, 0.0f, 1.0f);
		frame = static_cast<int>(curBlendPlayRatio * layerNode.GetBlendAllKeyFrame());
		if (ImGui::SliderInt("Frame", &frame, 0, layerNode.GetBlendAllKeyFrame()))
		{
			curBlendPlayRatio = static_cast<float>(frame) / layerNode.GetBlendAllKeyFrame();
		}
		ImGui::PopID();
	}
}