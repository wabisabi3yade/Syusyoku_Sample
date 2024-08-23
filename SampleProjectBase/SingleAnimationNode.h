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
	/// @param _transforms 格納するボーントランスフォーム配列
	/// @param _boneNum ボーンの数
	/// @param _requestKeyNum 取得したいキー数
	void GetAnimTransform(std::vector<BoneTransform>& _transforms, u_int _boneNum, u_int _requestKeyNum) const override;
};