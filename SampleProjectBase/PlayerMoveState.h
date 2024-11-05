#pragma once
#include "PlayerActState_Base.h"
#include "AnimationCurve.h"

class CP_Camera;

/// @brief プレイヤー移動処理
class PlayerMoveState : public PlayerActState_Base
{
protected:
	/// @brief 移動方向
	DirectX::SimpleMath::Vector3 moveVector;

	/// @brief 現在の速度
	float currentSpeed;

	/// @brief 最大速度
	float maxSpeed;

	/// @brief 加速度
	float acceleration;

	/// @brief 減速率
	float decadeSpeedTimes;

	/// @brief 回転速度
	float rotateSpeed;
public:
	PlayerMoveState();
	virtual ~PlayerMoveState() {}

	/// @brief セーブする
	/// @return セーブデータ
	nlohmann::json Save() override;

	/// @brief ロードする
	/// @param _data ロードするデータ 
	void Load(const nlohmann::json& _data) override;

protected:
	/// @brief 移動
	void Move();

	void ImGuiSetting() override;
private:
	void OnStartBehavior() override;
	void UpdateBehavior() override;
	void OnEndBehavior() override;
	void TransitionCheckUpdate() override;

	/// @brief ルートモーション適用
	void ApplyRootMotion();

	/// @brief 回転させる
	void Rotation();

	/// @brief 移動入力されているか
	bool IsMoveInput();

	/// @brief 走っているか取得
	/// @return 走っているか？
	bool IsRunning();
};

