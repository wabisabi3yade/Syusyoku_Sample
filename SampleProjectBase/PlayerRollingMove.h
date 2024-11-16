#pragma once
#include "PlayerActState_Base.h"
#include "AnimationCurve.h"

/// @brief プレイヤーのローリング処理
class PlayerRollingMove : public PlayerActState_Base
{
	/// @brief 移動距離の変位アニメーションカーブ
	std::unique_ptr<AnimationCurve> pDistanceCurve;

	/// @brief ローリング距離
	float rollingDistance;

	/// @brief 無敵時間
	float invicibleTime;

	/// @brief 経過時間
	float elapsedTime;

	/// @brief 前フレームまでに進んだ距離
	float prevProgressDistance;
public:
	PlayerRollingMove();
	~PlayerRollingMove() {}

private:
	/// @brief 各状態の開始処理
	void OnStartBehavior() override;

	/// @brief 更新処理
	void UpdateBehavior() override;

	/// @brief 終了処理
	void OnEndBehavior() override;

	/// @brief アニメーション終了時の行動
	/// @param _fromAnimNodeName 遷移元のアニメーションノード名
	/// @param _toAnimNodeName 遷移先のアニメーションノード名
	void OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName) override;

	/// @brief 入力方向に即時に向く
	void TurnInputVec();

	/// @brief 開始時のパラメータ処理
	void BeginParametar();

	/// @brief 前進方向に移動する
	void Move();

	/// @brief 無敵時間の更新
	void UpdateInvicible();

	void ImGuiDebug() override;

	/// @brief セーブする
	/// @return セーブデータ
	nlohmann::json Save() override;

	/// @brief ロードする
	/// @param _data ロードするデータ 
	void Load(const nlohmann::json& _data) override;
public:
	/// @brief ローリングトリガーのアニメーションパラメータ名
	static constexpr auto ROLLING_ANIMPARAM{ "rollingTrigger" };
};

