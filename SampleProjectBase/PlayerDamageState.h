#pragma once
#include "PlayerActState_Base.h"
#include "AnimationCurve.h"

/// @brief ダメージ処理
class PlayerDamageState : public PlayerActState_Base
{
	/// @brief 移動速度のアニメーショカーブ
	AnimationCurve knockSpeedCurve;

	/// @brief 吹っ飛ぶ方向(外部からセットする)
	DirectX::SimpleMath::Vector3 knockVector;

	/// @brief ダメージ中に移動する距離
	float maxKnockMoveSpeed;

	/// @brief 前回のアニメーション割合
	float prevAnimRatio;

	/// @brief ノックで移動しているか？
	bool isKnockMoving;

	/// @brief 無敵にするか？
	bool isInvicible;
public:
	PlayerDamageState();
	~PlayerDamageState() {}

	/// @brief 吹っ飛ぶベクトル
	void SetKnockVec(const DirectX::SimpleMath::Vector3& _knockVec);

	/// @brief セーブする
	/// @return セーブデータ
	nlohmann::json Save() override;

	/// @brief ロードする
	/// @param _data ロードするデータ 
	void Load(const nlohmann::json& _data) override;
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

	/// @brief ノック中の移動
	void KnockMove();

	void LookEnemy();

	void ImGuiDebug() override;
};

