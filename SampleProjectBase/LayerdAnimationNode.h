#pragma once
#include "AnimationNode_Base.h"

/// @brief 部位ごとに分けてアニメーションするノード
class LayerdAnimationNode : public AnimationNode_Base
{
	// 今回上半身、下半身の再生だけをするので2個だけ対応します、必要になったら拡張

	/// @brief 基礎となるアニメーション
	AnimationData* pBaseAnimation;

	/// @brief ブレンドするアニメーション
	AnimationData* pBlendAnimation;

	/// @brief どこからブレンドアニメーションを再生するか指定するボーンID
	int beginBlendBoneId;
public:
	/// @brief コンストラクタ
	/// @param _nodeName ノード名
	LayerdAnimationNode(const std::string& _nodeName);
	~LayerdAnimationNode() {}

	/// @brief アニメーションのトランスフォーム取得
	/// @param _outTransform 格納するボーントランスフォーム
	/// @param _boneId ボーンのID
	/// @param _requestRatio 取得する指定のアニメーション割合
	void GetAnimTransform(BoneTransform& _outTransform, u_int _boneId, float _requestRatio) const override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief 機能できているか確認する
	/// @return 機能できているか？
	bool CanWarking() const;

	void ImGuiDebug() override;

	// ブレンドのボーIDを取得する
	void ImGuiGetBlendBoneId();
};

