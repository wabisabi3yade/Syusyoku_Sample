#pragma once
#include "PlayerMoveState.h"

class PlayerTargetMove : public PlayerMoveState
{
	/// @brief 回転速度
	float rotateSpeed;

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
	void UpdateBehavior() override;
	void OnEndBehavior() override;
	void TransitionCheckUpdate() override;

	/// @brief 前方向を更新する
	void UpdateForward();

	/// @brief ブレンド割合をセット
	void ApplyBlendAnim();

	/// @brief ルートモーションをセット
	void ApplyRootMotion();

	void ImGuiDebug() override;
};

