#pragma once
#include "AnimationNode_Base.h"

/// @brief アニメーション情報がひとつだけのアニメーションノード
class SingleAnimationNode : public AnimationNode_Base
{
	/// @brief アニメーションデータ
	AnimationData* pAnimationData;
public:
	SingleAnimationNode(std::string _nodeName) : AnimationNode_Base(_nodeName, NodeType::Single), pAnimationData(nullptr) {}

	~SingleAnimationNode() {}

	void ImGuiPlaying() override;

	void Update(float _playingTime, BoneList& _boneList) override;

	/// @brief アニメーション
	/// @param _animData アニメーションデータ
	void SetAnimationData(const std::string& _animName) override;

	// アニメーションデータを取得する
	const AnimationData& GetAnimationData() const;

	/// @brief アニメーションのトランスフォーム取得
	/// @param _outTransform 格納するボーントランスフォーム
	/// @param _boneId ボーンのID
	/// @param _requestRatio 取得する指定のアニメーション割合
	void GetAnimTransform(BoneTransform& _outTransform, u_int _boneId, float _requestRatio) const override;
};