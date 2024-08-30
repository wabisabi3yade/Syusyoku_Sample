#pragma once
#include "AnimationController.h"
#include "IAnimationObserver.h"

// プレイヤーのアニメーションコントローラー
class PlayerAnimController : public AnimationController, public HashiTaku::AnimationSubject
{
public:
	enum class AnimType
	{
		Move,
		Attack,
		Jump,
		MaxNum
	};

private:
	/// @brief 現在のアニメーション状態
	AnimType nowState;

	/// @brief 速度割合
	float speedRatio;

public:
	PlayerAnimController();
	~PlayerAnimController() {}

	/// @brief アニメーション変更
	/// @param _type アニメーションのタイプ
	/// @param _type 補間処理を行うか?
	void ChangeAnimationByState(AnimType _type, bool _isInterp = true);

	/// @brief 移動速度割合をセット
	/// @param _speedRatio 移動速度割合
	void SetMoveSpeedRatio(float _speedRatio);

private:
	/// @brief 初期処理
	void Init();

	/// @brief アニメーション終了処理
	void OnAnimationFinish() override;
};

