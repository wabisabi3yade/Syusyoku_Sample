#pragma once
#include "AnimationNode_Base.h"

/// @brief アニメーション情報がひとつだけのアニメーションノード
class SingleAnimationNode : public AnimationNode_Base
{
	/// @brief アニメーションデータ
	AnimationData* pAnimationData;
public:
	SingleAnimationNode() : AnimationNode_Base("", NodeType::Single), pAnimationData(nullptr) {}
	SingleAnimationNode(std::string _nodeName) : AnimationNode_Base(_nodeName, NodeType::Single), pAnimationData(nullptr) {}

	~SingleAnimationNode() {}

	void ImGuiPlaying() override;
	void ImGuiSetting() override;

	void Update(float _playingTime, BoneList& _boneList) override;

	/// @brief アニメーション
	/// @param _animData アニメーションデータ
	void SetAnimationData(AnimationData& _animData) override;

	// アニメーションデータを取得する
	const AnimationData& GetAnimationData() const;

};