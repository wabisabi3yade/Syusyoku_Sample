#pragma once
#include "Asset_Base.h"

#include"AnimationNode_Base.h"
// 慣性補間
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

	/// @brief 再生割合
	float playingRatio;

	/// @brief 再生速度
	float playSpeed;

	// 仮
	float blendTime = 0.5f;

	// ブレンドしている時間
	float blendElapsedTime;

	/// @brief 再生するか
	bool isPlay;

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
	void Update(BoneList& _boneList);

	void ImGuiSetting();

	/// @brief アニメーション遷移する
	/// @param _animName アニメーション名
	/// @param  _isInterp 補間するか？
	void ChangeAnimation(const std::string& _animName, bool _isInterp = true);

	/// @brief ブレンド割合をセット
	/// @param _ratio 割合
	void SetBlendRatio(float _ratio);

	/// @brief シングルノードを作成
	/// @param _animName アニメーションの名前
	void CreateSingleNode(const std::string& _animName);

	/// @brief ブレンドノードを作成する
	/// @param _animNames アニメーションの名前
	/// @param _ratiosブレンド割合
	/// @param _nodeName ノード名
	void CreateBlendNode(const std::vector<std::string>& _animNames, const std::vector<float>& _ratios, const std::string& _nodeName);

	/// @brief アニメーションを除外
	/// @param _animName アニメーションの名前
	void RemoveAnimation(const std::string& _animName);

	/// @brief アニメーションがセットされているか返す
	/// @return アニメーションがセットされているか？
	bool IsSetAnimation();

	AnimationNode_Base* GetCurrentNode();
private:
	/// @brief 再生時間を進める
	void ProgressPlayTime();

	/// @brief 再生できる状態か？
	/// @return 再生できるか
	bool IsCanPlay();

	/// @brief アニメーションの更新処理
	/// @param _boneList 更新ボーンリスト
	void AnimatioUpdate(BoneList& _boneList);



	/// @brief ループ再生できるか？
	/// @return 再生できるフラグ
	bool IsCanLoop();

	/// @brief 通常時、アニメーション
	/// @param _boneList ボーンリスト
	void NormalUpdate(BoneList& _boneList);

	/// @brief 遷移時のアニメーション
	/// @param _boneList ボーンリスト
	void TransitionUpdate(BoneList& _boneList);

	void ImGuiTransition();

	void ImGuiImportAnim();

	/// @brief アニメーションを持ってるか返す
	/// @param _animName アニメーションの名前
	/// @return アニメーションがあるか？
	bool IsHaveAnim(const std::string& _animName);

	/// @brief 遷移開始する
	/// @param _animName アニメーション名
	void InterpTransitionStart(const std::string& _animName);

	/// @brief 遷移終了した時の処理
	void InterpTransitionEnd();

protected:
	/// @brief アニメーション終了時処理
	virtual void OnAnimationFinish();
};

