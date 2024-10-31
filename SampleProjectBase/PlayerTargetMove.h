#pragma once
#include "PlayerActState_Base.h"

class PlayerTargetMove : public PlayerActState_Base
{
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
public:
	PlayerTargetMove();
	~PlayerTargetMove() {}

	/// @brief セーブする
	/// @return セーブデータ
	nlohmann::json Save() override;

	/// @brief ロードする
	/// @param _data ロードするデータ 
	void Load(const nlohmann::json& _data) override;
private:
	void OnStartBehavior() override;
	void UpdateBehavior() override;
	void OnEndBehavior() override;

	/// @brief 移動
	void Move();

	/// @brief 走っているか取得
	/// @return 走っているか？
	bool IsRunning();

	void ImGuiSetting() override;

};

