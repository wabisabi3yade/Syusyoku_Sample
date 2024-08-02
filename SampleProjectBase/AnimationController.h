#pragma once

#include "AnimStateNode.h"

#include "Asset_Base.h"

/// @brief アニメーション遷移を管理するクラス
class AnimationController : public Asset_Base
{
	/// @brief アニメーションノード配列
	std::unordered_map<std::string, std::unique_ptr<AnimStateNode>> pAnimationNodes;

	/// @brief 再生中のアニメーション
	AnimStateNode* pCurrentAnimNode;

	// 仮
	float deltaChangeTime = 0.5f;

public:
	AnimationController() : pCurrentAnimNode(nullptr) {}
	~AnimationController() {}

	void ImGuiSetting();

	void ChangeAnimation(const std::string& _animName);
	
	/// @brief アニメーションを追加
	/// @param _animName アニメーションの名前
	void AddAnimation(const std::string& _animName);

	/// @brief アニメーションを除外
	/// @param _animName アニメーションの名前
	void RemoveAnimation(const std::string& _animName);

	/// @brief アニメーションがセットされているか返す
	/// @return アニメーションがセットされているか？
	bool IsSetAnimation();

	/// @brief 再生中のノードを取得する
	/// @return アニメーションノード
	AnimStateNode& GetCurrentNode();
private:

	/// @brief アニメーションを持ってるか返す
	/// @param _animName アニメーションの名前
	/// @return アニメーションがあるか？
	bool IsHaveAnim(const std::string& _animName);
};

