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

	/// @brief 遷移する先の攻撃
	PlayerState curChangeAtkState;

	/// @brief 攻撃情報リスト(単発なら最初の情報を使用する)
	std::vector<HashiTaku::AttackInformation> attackInfos;

	/// @brief 前に進む距離カーブ
	AnimationCurve progressDistanceCurve;

	/// @brief リアタック変数
	const bool* pIsReAttack;

	/// @brief 今回の攻撃で進む距離
	float curAtkProgressDis;

	/// @brief 進む最大距離
	float atkMaxDistance;

	/// @brief 前フレームまでに進んだ距離
	float prevProgressDistance;

	/// @brief 最初に見る回転速度
	float lookRotateSpeed;

	/// @brief 攻撃情報何個目か
	u_int curAttackTime;

	/// @brief 全体の攻撃情報何個あるか
	u_int attackTimeCnt;

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
	void TransitionCheckUpdate() override;

	void ImGuiDebug() override;
private:
	/// @brief パラメータを初期化
	void InitParameter();

	/// @brief 攻撃情報を更新する
	void UpdateAttackInfo();

	/// @brief リアタックするか確認する
	void UpdateReAttack();

	/// @brief コンビネーション攻撃の入力更新
	void UpdateCombInput();

	/// @brief 攻撃で進む距離を求める
	/// @param _atkEnemyPos 敵の座標
	void CalcProgressDis(const DirectX::SimpleMath::Vector3& _atkEnemyPos);

	/// @brief 敵に対して向ける
	/// @param _atkEnemyPos 敵の座標
	void LookAtEnemyInstant(DirectX::SimpleMath::Vector3 _atkEnemyPos);

	/// @brief 攻撃時に前へ進む
	void ForwardProgressMove();

	/// @brief 攻撃できるか取得する
	/// @return 攻撃できるか？
	bool GetCanCombAttack();

	/// @brief 攻撃する敵の座標を取得する
	/// @return 敵の座標
	DirectX::SimpleMath::Vector3 GetAtkEnemyPos();
public:
	/// @brief 攻撃トリガー
	constexpr static auto ATTACKTRIGGER_PARAMNAME{ "attackTrigger" };

	/// @brief リアタックするときに呼び出す変数
	static constexpr auto REATTACK_PARAMNAME{ "isReAttack" };
};