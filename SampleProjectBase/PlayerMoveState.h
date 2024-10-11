#pragma once
#include "PlayerActState_Base.h"

class CP_Camera;

/// @brief プレイヤー移動処理
class PlayerMoveState : public PlayerActState_Base
{
	/// @brief 移動方向
	DirectX::SimpleMath::Vector3 moveVector;

	CP_Camera* pCamera;

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
	~PlayerMoveState() {}

	/// @brief セーブする
	/// @return セーブデータ
	nlohmann::json Save() override;

	/// @brief ロードする
	/// @param _data ロードするデータ 
	void Load(const nlohmann::json& _data) override;
private:
	void OnStart() override;
	void Update() override;
	void OnEnd() override;

	/// @brief 移動
	void Move();

	DirectX::SimpleMath::Vector3 MoveVector();

	/// @brief 回転させる
	void Rotation();

	/// @brief	入力値を返す
	DirectX::SimpleMath::Vector2 InputValue();

	/// @brief 移動入力されているか
	bool IsMoveInput();

	/// @brief 走っているか取得
	/// @return 走っているか？
	bool IsRunning();

	void ImGuiSetting() override;
};

