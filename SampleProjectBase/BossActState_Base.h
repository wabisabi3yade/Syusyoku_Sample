#pragma once
#include "EnemyActState_Base.h"
#include "CP_Animation.h"
#include "CP_RigidBody.h"

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

		// 振る舞い
		Damage_Small,	// 小ダメージ

		// 攻撃
		CombAttack1 = 100,	// コンビネーション攻撃
		JumpAttack,	// ジャンプ攻撃

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

	/// @brief ダメージ時処理
	void OnDamage();

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
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
	/// @param _isForce　強制切り替え
	void ChangeState(BossState _nextState, bool _isForce = false);

	/// @brief トランスフォームを取得する
	/// @return ボスのトランスフォーム
	Transform& GetBossTransform();

	/// @brief トランスフォームを取得する
	/// @return プレイヤーのトランスフォーム
	Transform& GetPlayerTransform();

	/// @brief アニメーションコントローラポーネントを取得
	/// @return コンポーネント
	CP_Animation* GetAnimation();

	/// @brief Rbを取得
	/// @return Rbコンポーネント
	CP_RigidBody* GetRB();

	/// @brief 経過時間を取得する
	/// @return 経過時間
	float DeltaTime() const;

	void ImGuiDebug() override;
private:
	/// @brief どのアクションにも共通する遷移
	void CommmonCheckTransition();

protected:
	// パラメータ
	/// @brief 移動速度のアニメーションパラメータ名
	static constexpr auto SPEED_ANIMPARAM_NAME{ "speed" };
	/// @brief 攻撃トリガーのアニメーションパラメータ名
	static constexpr auto ATTACKTRIGGER_ANIMPARAM_NAME{ "attackTrigger" };
	/// @brief 攻撃トリガーのアニメーションパラメータ名
	static constexpr auto DAMAGETRIGGER_ANIMPARAM_NAME{ "damageTrigger" };

	// アニメーション
	/// @brief 待機状態のアニメーション名
	static constexpr auto IDLE_ANIM_NAME{ "Idle" };
	/// @brief 走り状態のアニメーション名
	static constexpr auto RUN_ANIM_NAME{ "Move" };
};
