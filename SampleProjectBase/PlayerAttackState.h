#pragma once
#include "PlayerActState_Base.h"
#include "AttackInformation.h"
#include "AnimationCurve.h"

/// @brief プレイヤー攻撃のゲーム内処理
class PlayerAttackState : public PlayerActState_Base
{
private:
	/// @brief コンビネーション攻撃の先
	PlayerState nextCombAtkState;

	/// @brief 攻撃情報
	std::unique_ptr<HashiTaku::AttackInformation> pAttackInfo;

	/// @brief 前に進む距離カーブ
	AnimationCurve progressDistanceCurve;

	/// @brief 進む距離
	float atkMoveSpeed;

	/// @brief 最初に見る回転速度
	float lookRotateSpeed;

	/// @brief 前へ進むか？
	bool isMoveForward;

	/// @brief 攻撃判定出る前か？
	bool isAttackCollisionBefore; 
	
public:
	PlayerAttackState();
	virtual ~PlayerAttackState() {}

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
protected:
	// State共通処理
	void OnStartBehavior() override;
	void UpdateBehavior() override;
	void OnEndBehavior() override;
	void TransitionCheckUpdate() override;

	void ImGuiDebug() override;
private:
	/// @brief 攻撃情報を更新する
	void UpdateAttackInfo();

	/// @brief 敵に対して向ける
	void LookAtEnemyInstant();

	/// @brief 攻撃時に前へ進む
	void ForwardProgressMove();
public:
	constexpr static auto ATTACKTRIGGER_PARAMNAME = "attackTrigger";	// 攻撃トリガー
};