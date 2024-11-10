#pragma once
#include "BossGroundState.h"

/// @brief ボス地上移動
class BossGroundMove : public BossGroundState
{
	/// @brief  最大移動速度
	float maxSpeed;

	/// @brief 加速度
	float acceleration;

	/// @brief 現在速度
	float currentSpeed;

	/// @brief 回転速度
	float rotateSpeed;
	
	/// @brief 近距離攻撃に移行する距離
	float transNearDistance;

public:
	BossGroundMove();
	~BossGroundMove() {}

	void DebugDisplay() override;

	nlohmann::json Save();
	void Load(const nlohmann::json& _data);
private:
	/// @brief 更新処理
	void UpdateBehavior() override;

	/// @brief 状態切り替え終了処理
	void OnEndBehavior() override;

	/// @brief 遷移切り替え
	void TransitionCheckUpdate() override;

	/// @brief 自身を回転する
	void Rotation();

	/// @brief 移動処理
	void Move();

	/// @brief 近接の距離になっているか確認
	/// @return 遷移するか？
	bool CheckNearTransition();

	void ImGuiDebug() override;
};

