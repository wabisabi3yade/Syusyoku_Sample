#pragma once
#include "EnemyActState_Base.h"

class BossActionController;

/// @brief ボスのステートノード基底
class BossActState_Base : public EnemyActState_Base
{
public:
	// ボスのステートタイプ
	enum class BossState
	{
		// 移動
		Idle,
		Walk,
		Move,

		// 攻撃
		Attack1,

		// 最後
		None
	};

private:
	/// @brief ボスのステート
	BossState stateType;

	/// @brief アクションコントローラー
	BossActionController* pActionController;

public:
	BossActState_Base();
	virtual ~BossActState_Base() {}

	/// @brief 初期化処理
	/// @param _stateType　状態
	/// @param _actController　アクションコントローラー
	void Init(BossState _stateType, BossActionController& _actController);

	/// @brief 状態切り替え開始処理
	void OnStart() override;

	/// @brief 更新処理
	void Update() override;

	/// @brief 状態切り替え終了処理
	void OnEnd() override;
};

