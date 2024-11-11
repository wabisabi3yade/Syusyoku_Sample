#pragma once
#include "IAnimInterpolate.h"

class AnimNodePlayer_Base;
class BoneList;
struct BoneTransform;

/// @brief アニメーション間の補間に使用するクロスフェード補間クラス
class CrossFadeAnimation : public IAnimInterpolate
{
	/// @brief 遷移元アニメーション
	AnimNodePlayer_Base* pFromNodePlayer;

	/// @brief 遷移先アニメーション
	AnimNodePlayer_Base* pToNodePlayer;

	BoneList* pAssetBoneList;

	/// @brief 遷移のウェイト
	float transitionWeight;

	/// @brief 遷移時間
	float transitionTime;

	/// @brief 現在の遷移時間
	float elapsedTime;

	/// @brief イージング種類
	HashiTaku::EaseKind easeKind;
public:
	CrossFadeAnimation();
	~CrossFadeAnimation() {}

	/// @brief 開始処理
	/// @param _fromNode 遷移元アニメーションノード
	/// @param _toNode 遷移先アニメーションノード
	/// @param _updateBones　更新するボーンリスト 
	/// @param  _transitionTime 遷移時間 
	/// @param _fromStartRatio 遷移開始時の遷移元のアニメーション割合
	/// @param _toStartRatio 遷移開始時の遷移先のアニメーション割合
	/// @param _easeKind ウエイトを変化させるときのイージング
	void Begin(AnimNodePlayer_Base& _fromNode, AnimNodePlayer_Base& _toNode, BoneList& _updateBones, float _transitionTime, HashiTaku::EaseKind _easeKind);

	/// @brief 更新処理
	/// @param _playSpeed アニメーション全体速度
	void Update(float _playSpeed);

	/// @brief 遷移時間を進める
	/// @param _deltaTime 再生速度を考慮した経過時間
	void ProgressTime(float _deltaTime);

	/// @brief 遷移終了したか取得
	/// @return 遷移終了したか？
	bool GetIsTransitionEnd() const;

	// 遷移経過時間を取得する
	float GetElapsedTime() const override;

	// 遷移時間を取得 
	float GetTransitionTime() const override;

	/// @brief 遷移ウェイトを取得
	/// @return 遷移ウェイト
	float GetTransitionWeight() const;
private:
	/// @brief トランジションによる補間処理
	/// @param _fromTransforms 遷移元のトランスフォーム
	/// @param _toTransforms 遷移先のトランスフォーム
	/// @param _outTransforms 補間したトランスフォーム
	void Interpolate(const BoneTransform& _fromTransforms, const BoneTransform& _toTransforms, BoneTransform& _outTransforms);
};

