#pragma once
#include "PlayerMoveState.h"

class PlayerTargetMove : public PlayerMoveState
{
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
	void TransitionCheckUpdate() override;

	/// @brief ブレンド割合をセット
	void ApplyBlendAnim();

	/// @brief ルートモーションをセット
	void ApplyRootMotion();

	void ImGuiSetting() override;
};

