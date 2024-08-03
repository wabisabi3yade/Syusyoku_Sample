#pragma once

#include "AnimationData.h"

// アニメーション遷移で使用する1ノードクラス
class AnimStateNode
{
	/// @brief ノード名
	std::string nodeName;

	/// @brief アニメーションデータ
	AnimationData* pAnimationData;

	/// @brief ループ再生するか？
	bool isLoop;
public:
	AnimStateNode() : nodeName(""), pAnimationData(nullptr), isLoop(true) {}
	AnimStateNode(std::string _nodeName) : nodeName(_nodeName), pAnimationData(nullptr), isLoop(true) {}
	~AnimStateNode() {}

	void ImGuiSetting();

	/// @brief アニメーション
	/// @param _animData アニメーションデータ
	void SetAnimationData(AnimationData& _animData);

	// ノード名取得
	std::string GetNodeName() const;

	// アニメーションデータを取得する
	const AnimationData& GetAnimationData() const;

	/// @brief ループ再生フラグを取得
	/// @return ループ再生フラグ
	bool GetIsLoop() const;
};