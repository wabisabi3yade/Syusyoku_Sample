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
		Run,

		// 攻撃
		Attack1 = 100,
		Attack2,

		// 最後
		None
	};

private:
	/// @brief ボスのステート
	BossState stateType;


protected:
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

	nlohmann::json Save();
	void Load(const nlohmann::json& _data);
protected:
	/// @brief 状態切り替え開始処理
	virtual void OnStartBehavior() {}

	/// @brief 更新処理
	virtual void UpdateBehavior() {}

	/// @brief 状態切り替え終了処理
	virtual void OnEndBehavior() {}

	/// @brief 遷移切り替え確認
	virtual void TransitionCheckUpdate() {}

	/// @brief 遷移する
	/// @param _nextState 次の遷移
	void ChangeState(BossState _nextState);

	/// @brief トランスフォームを取得する
	/// @return ボスのトランスフォーム
	Transform& GetBossTransform();

	/// @brief トランスフォームを取得する
	/// @return プレイヤーのトランスフォーム
	Transform& GetPlayerTransform();

	void ImGuiSetting() override;

protected:
	/// @brief 移動速度のアニメーションパラメータ名
	static constexpr auto SPEED_ANIMPARAM_NAME{ "speed" };
	/// @brief 攻撃トリガーのアニメーションパラメータ名
	static constexpr auto ATTACKTRIGGER_ANIMPARAM_NAME{ "attackTrigger" };
	/// @brief 待機状態のアニメーション名
	static constexpr auto RUN_ANIM_NAME{ "Move" };
};
