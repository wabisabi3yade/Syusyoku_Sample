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

	void Update(BoneList& _boneList) override;

	/// @brief アニメーション
	/// @param _animData アニメーションデータ
	void SetAnimationData(const std::string& _animName) override;

	// アニメーションデータを取得する
	const AnimationData& GetAnimationData() const;

	/// @brief 現在のアニメーションのトランスフォーム取得
	/// @param _outTransform 格納するボーントランスフォーム
	/// @param _boneId ボーンのID
	void GetCurAnimTransform(BoneTransform& _outTransform, u_int _boneId) const override;

	/// @brief アニメーションのトランスフォーム取得
	/// @param _outTransform 格納するボーントランスフォーム
	/// @param _boneId ボーンのID
	/// @param _requestRatio 取得する指定のアニメーション割合
	void GetAnimTransform(BoneTransform& _outTransform, u_int _boneId, float _requestRatio) const override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;

	// 再生時に表示する
	void ImGuiPlaying() override;
private:
	void ImGuiSetting() override;
};