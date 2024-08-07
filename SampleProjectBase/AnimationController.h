#pragma once
#include "Asset_Base.h"

#include "AnimationNode_Base.h"

#include "InertInterpAnimation.h"
// vector配列で使用するためinclude
#include "Bone.h"

// ボーン
class BoneList;

/// @brief アニメーション遷移を管理するクラス
class AnimationController : public Asset_Base
{
	/// @brief 遷移先のアニメーション
	AnimationNode_Base* pNextAnimNode;

	/// @brief 慣性補間
	std::unique_ptr<InertInterpAnimation> inertInterp;

	// 仮
	float blendTime = 0.5f;

	// ブレンドしている時間
	float blendElapsedTime = 0.0f;

	/// @brief アニメーション遷移中か？
	bool isTransitioning;

protected:
	/// @brief アニメーションノード配列
	std::unordered_map<std::string, std::unique_ptr<AnimationNode_Base>> pAnimationNodes;

	/// @brief 再生中のアニメーション
	AnimationNode_Base* pCurrentAnimNode;

public:
	AnimationController();
	~AnimationController() {}

	/// @brief ボーンのアニメーションを更新する
	/// @param _boneList ボーンリスト
	/// @param _playingTime 再生時間
	void Update(BoneList& _boneList, float _playingTime);

	/// @brief 通常時、アニメーション
	/// @param _boneList ボーンリスト
	/// @param _playingTime 再生時間
	/// @param _cacheTransform キャッシュ用の配列参照
	void NormalUpdate(BoneList& _boneList, float& _playingTime);

	/// @brief 遷移時のアニメーション
	/// @param _boneList ボーンリスト
	/// @param _playingTime 再生時間
	/// @param _cacheTransform キャッシュ用の配列参照
	void TransitionUpdate(BoneList& _boneList, float& _playingTime);

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

	AnimationNode_Base* GetCurrentNode();
private:
	void ImGuiTransition();

	void ImGuiImportAnim();

	/// @brief アニメーションを持ってるか返す
	/// @param _animName アニメーションの名前
	/// @return アニメーションがあるか？
	bool IsHaveAnim(const std::string& _animName);

	/// @brief 遷移終了した時の処理
	void TransitionEnd(float& _playTime);
};

