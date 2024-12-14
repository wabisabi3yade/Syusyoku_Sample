#pragma once
#include "BossActState_Base.h"
#include "AttackInformation.h"

/// @brief ボスのs攻撃ステート
class BossAttackState : public BossActState_Base
{
	/// @brief 回転速度カーブ
	AnimationCurve rotSpeedCurve;

	/// @brief 回転速度倍率
	float rotSpeedTimes;

	/// @brief コンビネーション攻撃の攻撃回数
	u_int attackTimeCnt;

	/// @brief 現在、攻撃の何段目か
	u_int curAttackTime;

	/// @brief カーブで回転移動させるか？
	bool isUseRotateCurve;

protected :
	/// @brief 攻撃情報リスト(単発なら最初の情報を使用する)
	std::vector<HashiTaku::AttackInformation> attackInfos;

public:
	BossAttackState();
	virtual ~BossAttackState() {}

	/// @brief 開始
	void OnStartBehavior() override;

	/// @brief 更新処理
	void UpdateBehavior() override;

	/// @brief アニメーション終了時の行動
	/// @param _fromAnimNodeName 遷移元のアニメーションノード名
	/// @param _toAnimNodeName 遷移先のアニメーションノード名
	void OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName) override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
protected:
	/// @brief 向きを更新する
	void RotateUpdate();

	void ImGuiDebug() override;

private:
	/// @brief コンビネーション攻撃の攻撃に合わせて攻撃情報を更新する
	void UpdateReAttack();

	/// @brief 攻撃回数をセット
	/// @param _attackTime 攻撃回数
	void SetAttackTimeCnt(u_int _attackTime);
};

