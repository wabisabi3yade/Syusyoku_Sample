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
	std::unordered_map<AnimType, std::string> animTypeNodeNames;

	/// @brief 速度割合
	float speedRatio;

	AnimType nowState;

public:
	PlayerAnimController();
	~PlayerAnimController() {}

	/// @brief 初期のanimationセット
	/// @param _type アニメーションのタイプ
	void SetStartAnimation(AnimType _type);

	/// @brief 移動速度割合をセット
	/// @param _speedRatio 移動速度割合
	void SetMoveSpeedRatio(float _speedRatio);

private:
	/// @brief 初期処理
	void Init();

	/// @brief ノード作成
	void InitCreateNode();

	/// @brief 遷移矢印を作成
	void InitTransitionArrow();

	/// @brief アニメ種類の列挙型とノード名を対応づける
	/// @param _animType アニメ種類
	/// @param _nodeName アニメノード名
	void LinkAnimTypeNodeName(AnimType _animType, const std::string& _nodeName);

	/// @brief アニメーション終了処理
	void OnAnimationFinish() override;

	/// @brief アニメーション遷移する
	/// @param _animName アニメーション名
	/// @param  _isInterp 補間するか？
	virtual void ChangeAnimation(const std::string& _animName, float _targetAnimRatio = 0.0f, bool _isInterp = true);


	/// @brief 名前から列挙型を探す
	/// @param _nodeName ノード名
	AnimType FindAnimType(const std::string& _nodeName);

	void ImGuiSetting() override;
};

